# !usr/bin/env python3
# -*- coding: utf-8 -*-
# Author:liukanglai
# Blog:
# Time:2022-06-11 09:29:55
# Name:figureCude.py
# Version:V1.0

# 使用CIFAR-10数据集进行图像分类
# 由于ImageNet数据集较大，下载和训练较慢，为了方便大家学习，我们使用CIFAR10数据集。CIFAR10数据集包含60,000张32x32的彩色图片，10个类别，每个类包含6,000张。其中50,000张图片作为训练集，10000张作为测试集。图11从每个类别中随机抽取了10张图片，展示了所有的类别。
# 数据集中的图像大小为32x32x3 。在训练的过程中最好使用GPU来加速。
import torch
import numpy as np

# 检查是否可以利用GPU
train_on_gpu = torch.cuda.is_available()

if not train_on_gpu:
    print('CUDA is not available.')
else:
    print('CUDA is available!')

# 加载数据
# 数据下载可能会比较慢。请耐心等待。加载训练和测试数据，将训练数据分为训练集和验证集，然后为每个数据集创建DataLoader
from torchvision import datasets
import torchvision.transforms as transforms
from torch.utils.data.sampler import SubsetRandomSampler

# number of subprocesses to use for data loading
num_workers = 0
# 每批加载16张图片
batch_size = 16
# percentage of training set to use as validation
# 验证集
valid_size = 0.2

# 将数据转换为torch.FloatTensor，并标准化。
# 下载数据集并对图片进行调整, 因为torchvision数据集的输出是PILImage格式, 数据域在[0, 1]. 我们将其转换为标准数据域[-1, 1]的张量格式

# 定义归一化方法
transform = transforms.Compose([
    # 首先装换数据为tensor张量
    transforms.ToTensor(),
    # 对数据进行正态分布归一化，RGB三个通道每个通道均值为0.5，标准差为0.5
    transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))
])

# 选择训练集与测试集的数据
# data为数据存放的目录，train=Ture表示训练集，download=True表示要下载，transform为之前定义的归一化方法
train_data = datasets.CIFAR10('data',
                              train=True,
                              download=True,
                              transform=transform)
test_data = datasets.CIFAR10('data',
                             train=False,
                             download=True,
                             transform=transform)

# obtain training indices that will be used for validation
num_train = len(train_data)
indices = list(range(num_train))
np.random.shuffle(indices)
split = int(np.floor(valid_size * num_train))
train_idx, valid_idx = indices[split:], indices[:split]

# define samplers for obtaining training and validation batches
train_sampler = SubsetRandomSampler(train_idx)
valid_sampler = SubsetRandomSampler(valid_idx)

# prepare data loaders (combine dataset and sampler)
# 定义数据集的加载方法， train_data为训练集， batch_size为单词训练的样本数，sampler表示样本选择（此为随机），num_workers表示加载的线程数
train_loader = torch.utils.data.DataLoader(train_data,
                                           batch_size=batch_size,
                                           sampler=train_sampler,
                                           num_workers=num_workers)
valid_loader = torch.utils.data.DataLoader(train_data,
                                           batch_size=batch_size,
                                           sampler=valid_sampler,
                                           num_workers=num_workers)
test_loader = torch.utils.data.DataLoader(test_data,
                                          batch_size=batch_size,
                                          num_workers=num_workers)

# 图像分类中10类别
classes = [
    'airplane', 'automobile', 'bird', 'cat', 'deer', 'dog', 'frog', 'horse',
    'ship', 'truck'
]

# 查看训练集中的一批样本
import matplotlib.pyplot as plt

# %matplotlib inline use in Ipython


# helper function to un-normalize and display an image
# 构建展示图片的函数
def imshow(img):
    img = img / 2 + 0.5  # unnormalize
    plt.imshow(np.transpose(img, (1, 2, 0)))  # convert from Tensor image


# 获取一批样本
# 从数据迭代器中读取一张图片
dataiter = iter(train_loader)
images, labels = dataiter.next()
images = images.numpy()  # convert images to numpy for display

# 显示图像，标题为类名
fig = plt.figure(figsize=(25, 4))
# 显示16张图片
for idx in np.arange(16):
    ax = fig.add_subplot(2, (int)(16 / 2), idx + 1, xticks=[], yticks=[])
    imshow(images[idx])
    ax.set_title(classes[labels[idx]])

plt.show()

import torch.nn as nn
import torch.nn.functional as F


# 定义卷积神经网络结构，一个简单的网络类
class Net(nn.Module):

    def __init__(self):
        super(Net, self).__init__()
        # 第一层卷积神经网络, 输入通道维度=3, 输出通道维度=6, 卷积核大小3*3, 填充为1 (32*32*3的图像)
        self.conv1 = nn.Conv2d(3, 16, 3, padding=1)
        # 卷积层(16*16*16)
        self.conv2 = nn.Conv2d(16, 32, 3, padding=1)
        # 卷积层(8*8*32)
        self.conv3 = nn.Conv2d(32, 64, 3, padding=1)

        # 最大池化层
        self.pool = nn.MaxPool2d(2, 2)

        # 定义三层全连接网络
        # linear layer (64 * 4 * 4 -> 500)
        self.fc1 = nn.Linear(64 * 4 * 4, 500)
        # linear layer (500 -> 10)
        self.fc2 = nn.Linear(500, 10)
        # dropout层 (p=0.3)
        # dropout 是指在深度学习网络的训练过程中，按照一定的概率将一部分神经网络单元暂时从网络中丢弃，相当于从原始的网络中找到一个更瘦的网络
        self.dropout = nn.Dropout(0.3)

    # 定义数据流向
    def forward(self, x):
        # 在(2, 2)的池化窗口下执行最大池化操作
        # add sequence of convolutional and max pooling layers
        x = self.pool(F.relu(self.conv1(x)))
        x = self.pool(F.relu(self.conv2(x)))
        x = self.pool(F.relu(self.conv3(x)))

        # flatten image input
        # 调整数据维度，‘-1’表示自动计算维度
        # 64*4*4矩阵展平成一维的再传入全连接层(x.view(-1,50*75))
        x = x.view(-1, 64 * 4 * 4)

        # add dropout layer
        x = self.dropout(x)

        # add 1st hidden layer, with relu activation function
        # 数据经过激活函数
        x = F.relu(self.fc1(x))

        # add dropout layer
        x = self.dropout(x)

        # add 2nd hidden layer, with relu activation function
        x = self.fc2(x)
        return x


# create a complete CNN
# 新建一个网络model
model = Net()
print(model)

# 查看模型中所有的可训练参数
# params = list(model.parameters())
# print(len(params))
# print(params[0].size())

# 使用GPU
if train_on_gpu:
    model.cuda()

# 选择损失函数与优化函数
# 导入torch中优化器相关的包
import torch.optim as optim
# 使用交叉熵损失函数
criterion = nn.CrossEntropyLoss()
# 使用随机梯度下降，学习率lr=0.01
optimizer = optim.SGD(model.parameters(), lr=0.01)

# 根据训练数据训练卷积神经网络模型
#  计算损失值, 将网络参数的梯度进行反向传播, 以一定的规则更新网络的权重, 需要遍历数据迭代器, 然后将数据输入网络并进行优化。
# 注意：训练集和验证集的损失是如何随着时间的推移而减少的；如果验证损失不断增加，则表明可能过拟合现象。（实际上，在下面的例子中，如果n_epochs设置为40，可以发现存在过拟合现象！）
# 训练模型的次数
n_epochs = 30

valid_loss_min = np.Inf  # track change in validation loss

for epoch in range(1, n_epochs + 1):

    # keep track of training and validation loss
    train_loss = 0.0
    valid_loss = 0.0

    ###################
    # 训练集的模型 #
    ###################
    model.train()
    for data, target in train_loader:
        # move tensors to GPU if CUDA is available
        if train_on_gpu:
            # 获得数据与标签
            data, target = data.cuda(), target.cuda()
        # clear the gradients of all optimized variables
        # optimizer.zero_grad()	# 清理上一次循环的梯度
        optimizer.zero_grad()
        # forward pass: compute predicted outputs by passing inputs to the model
        # 得到网络的输出
        output = model(data)
        # calculate the batch loss
        loss = criterion(output, target)
        # backward pass: compute gradient of the loss with respect to model parameters
        # 反向传播
        loss.backward()
        # perform a single optimization step (parameter update)
        # 更新参数
        optimizer.step()
        # update training loss
        train_loss += loss.item() * data.size(0)

    ######################
    # 验证集的模型#
    ######################
    model.eval()
    for data, target in valid_loader:
        # move tensors to GPU if CUDA is available
        if train_on_gpu:
            data, target = data.cuda(), target.cuda()
        # forward pass: compute predicted outputs by passing inputs to the model
        output = model(data)
        # calculate the batch loss
        loss = criterion(output, target)
        # update average validation loss
        valid_loss += loss.item() * data.size(0)

    # 计算平均损失
    train_loss = train_loss / len(train_loader.sampler)
    valid_loss = valid_loss / len(valid_loader.sampler)

    # 显示训练集与验证集的损失函数
    print('Epoch: {} \tTraining Loss: {:.6f} \tValidation Loss: {:.6f}'.format(
        epoch, train_loss, valid_loss))

    # 可以发现，随着训练数据的增加，Loss 有明显减少的趋势

    # 如果验证集损失函数减少，就保存模型。
    if valid_loss <= valid_loss_min:
        print(
            'Validation loss decreased ({:.6f} --> {:.6f}).  Saving model ...'.
            format(valid_loss_min, valid_loss))
        # 保存模型参数到路径'./model.pt'中
        torch.save(model.state_dict(), 'model_cifar.pt')
        valid_loss_min = valid_loss

# 加载模型
model.load_state_dict(torch.load('model_cifar.pt'))

# 测试训练好的网络
# 在测试数据上测试你的训练模型！一个“好”的结果将是CNN得到大约70%，这些测试图像的准确性。
# track test loss
test_loss = 0.0
class_correct = list(0. for i in range(10))
class_total = list(0. for i in range(10))

model.eval()
# iterate over test data
for data, target in test_loader:
    # move tensors to GPU if CUDA is available
    if train_on_gpu:
        data, target = data.cuda(), target.cuda()
    # forward pass: compute predicted outputs by passing inputs to the model
    output = model(data)
    # calculate the batch loss
    loss = criterion(output, target)
    # update test loss
    test_loss += loss.item() * data.size(0)
    # convert output probabilities to predicted class
    _, pred = torch.max(output, 1)
    # compare predictions to true label
    correct_tensor = pred.eq(target.data.view_as(pred))
    correct = np.squeeze(
        correct_tensor.numpy()) if not train_on_gpu else np.squeeze(
            correct_tensor.cpu().numpy())
    # calculate test accuracy for each object class
    for i in range(batch_size):
        label = target.data[i]
        class_correct[label] += correct[i].item()
        class_total[label] += 1

# average test loss
test_loss = test_loss / len(test_loader.dataset)
print('Test Loss: {:.6f}\n'.format(test_loss))

for i in range(10):
    if class_total[i] > 0:
        print('Test Accuracy of %5s: %2d%% (%2d/%2d)' %
              (classes[i], 100 * class_correct[i] / class_total[i],
               np.sum(class_correct[i]), np.sum(class_total[i])))
    else:
        print('Test Accuracy of %5s: N/A (no training examples)' %
              (classes[i]))

print('\nTest Accuracy (Overall): %2d%% (%2d/%2d)' %
      (100. * np.sum(class_correct) / np.sum(class_total),
       np.sum(class_correct), np.sum(class_total)))

# 显示测试样本的结果
# obtain one batch of test images
dataiter = iter(test_loader)
images, labels = dataiter.next()
images.numpy()

# move model inputs to cuda, if GPU available
if train_on_gpu:
    images = images.cuda()

# get sample outputs
output = model(images)
# convert output probabilities to predicted class
_, preds_tensor = torch.max(output, 1)
preds = np.squeeze(preds_tensor.numpy()) if not train_on_gpu else np.squeeze(
    preds_tensor.cpu().numpy())

# plot the images in the batch, along with predicted and true labels
fig = plt.figure(figsize=(25, 4))
for idx in np.arange(16):
    ax = fig.add_subplot(2, (int)(16 / 2), idx + 1, xticks=[], yticks=[])
    imshow(images.cpu()[idx])
    ax.set_title(
        "{} ({})".format(classes[preds[idx]], classes[labels[idx]]),
        color=("green" if preds[idx] == labels[idx].item() else "red"))
plt.show()
