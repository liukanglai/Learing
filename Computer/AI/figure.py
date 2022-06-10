# !usr/bin/env python3
# -*- coding: utf-8 -*-
# Author:liukanglai
# Blog:
# Time:2022-06-10 10:44:15
# Name:figure.py
# Version:V1.0

# 导入若干工具包
import torch
import torch.nn as nn
import torch.nn.functional as F


# 定义卷积神经网络
# 定义一个简单的网络类
class Net(nn.Module):

    def __init__(self):
        super(Net, self).__init__()
        # 定义第一层卷积神经网络, 输入通道维度=1, 输出通道维度=6, 卷积核大小5*5
        self.conv1 = nn.Conv2d(3, 6, 5)
        self.conv2 = nn.Conv2d(6, 16, 5)
        # 定义三层全连接网络
        self.fc1 = nn.Linear(16 * 5 * 5, 120)  # 5*5 from image dimension
        self.fc2 = nn.Linear(120, 84)
        self.fc3 = nn.Linear(84, 10)

    def forward(self, x):
        # 在(2, 2)的池化窗口下执行最大池化操作
        x = F.max_pool2d(F.relu(self.conv1(x)), (2, 2))
        # If the size is a square you can only specify a single number
        x = F.max_pool2d(F.relu(self.conv2(x)), 2)
        x = x.view(-1, 16 * 5 * 5)
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = self.fc3(x)
        return x


net = Net()
print(net)

# 模型中所有的可训练参数
params = list(net.parameters())
print(len(params))
print(params[0].size())

# 加载和标准化数据
# 加载数据
# 下载数据集并对图片进行调整, 因为torchvision数据集的输出是PILImage格式, 数据域在[0, 1]. 我们将其转换为标准数据域[-1, 1]的张量格式.
import torchvision
import torchvision.transforms as transforms

transform = transforms.Compose([
    transforms.ToTensor(),
    transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))
])

trainset = torchvision.datasets.CIFAR10(root='./data',
                                        train=True,
                                        download=True,
                                        transform=transform)
trainloader = torch.utils.data.DataLoader(trainset,
                                          batch_size=4,
                                          shuffle=True,
                                          num_workers=2)

testset = torchvision.datasets.CIFAR10(root='./data',
                                       train=False,
                                       download=True,
                                       transform=transform)
testloader = torch.utils.data.DataLoader(testset,
                                         batch_size=4,
                                         shuffle=False,
                                         num_workers=2)

classes = ('plane', 'car', 'bird', 'cat', 'deer', 'dog', 'frog', 'horse',
           'ship', 'truck')

# 展示图片
# 导入画图包和numpy
import matplotlib.pyplot as plt
import numpy as np


# 构建展示图片的函数
def imshow(img):
    img = img / 2 + 0.5
    npimg = img.numpy()
    plt.imshow(np.transpose(npimg, (1, 2, 0)))
    plt.show()


# 从数据迭代器中读取一张图片
dataiter = iter(trainloader)
images, labels = dataiter.next()

# 展示图片
imshow(torchvision.utils.make_grid(images))
# 打印标签label
print(' '.join('%5s' % classes[labels[j]] for j in range(4)))

# 定义损失函数和优化器
# 使用分类交叉熵损失和带有动量的 SGD
import torch.optim as optim

criterion = nn.CrossEntropyLoss()
optimizer = optim.SGD(net.parameters(), lr=0.001, momentum=0.9)

# 根据训练数据训练网络
# 计算损失值
# 将网络参数的梯度进行反向传播
# 以一定的规则更新网络的权重
# 需要遍历数据迭代器, 然后将数据输入网络并进行优化。
for epoch in range(5):

    running_loss = 0.0
    for batch_i, (imgs, target) in enumerate(trainloader):
        batches_done = len(trainloader) * epoch + batch_i

        output = net(imgs)
        loss = criterion(output, target)

        optimizer.zero_grad()
        loss.backward()
        optimizer.step()

        running_loss += loss.item()
        if (batch_i + 1) % 2000 == 0:  # print every 2000 mini-batches
            print('[%d, %5d] loss: %.3f' %
                  (epoch + 1, batch_i + 1, running_loss / 2000))
            running_loss = 0.0

    if (epoch + 1) % 5 == 0:
        torch.save(net.state_dict(), 'checkpoints/class_%d.pth' %
                   epoch)  # 模型保存在 checkpoints 文件夹下, 请提前创建此文件夹.
        print('save')
print('Finished Training')

# 使用模型
dataiter = iter(testloader)
images, labels = dataiter.next()

net = Net()
net.load_state_dict(torch.load('checkpoints/class_4.pth'))
outputs = net(images)
_, predicted = torch.max(outputs, 1)
print('GroundTruth: ', ' '.join('%5s' % classes[labels[j]] for j in range(4)))
print('Predicted: ', ' '.join('%5s' % classes[predicted[j]] for j in range(4)))
