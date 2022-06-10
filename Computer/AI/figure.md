# 图像分类

实现一个简单的图像分类器, 本篇博客主要分为以下几个步骤：数据的加载与归一、定义神经网络、定义损失函数、训练与测试神经网络以及神经网络存储与读取。

1. 数据加载
   数据加载就是把训练数据导入到神经网络中并对神经网络进行训练，图像分类器训练数据一般比较大，无法一次性加载所有数据，例如：

CIFAR10 数据集含有 10 个类共计 6 万张图片
ImageNet 数据集含有 1000 个类超过 100 万张图片
因为数据集比较大，所以一般需要用 mini-batch 形式进行加载并训练：

每个 mini-batch 只加载所有训练数据集中的一部分数据
任意两个 mini-batch 之间的数据不重叠
当所有的训练数据集中的数据都被加载并训练完一次被称作一个 epoch
因为图像像素值比较大，所以需要对数据归一化：

图像数据像素值一般在[0-255]之间
在训练神经网络时，要把输入数据值变成[0-1]或者[-1-1]之间
与数据加载与归一化相关的 PyTorch 库为：

数据加载：
torchvision.dataset
数据归一：
torchvision.transforms

1.1 常用公共数据集加载

- 本篇博客所用到的数据库为 CIFAR10 数据库, 一共有 10 类，每类图片有 6000 张，图像参数为：

  大小： 32x32x3
  通道：R,G,B 三个通道
  像素：每个通道有 32x32 个像素

```
# 导入相关pkg
import torch
import torchvision
import torchvision.transforms as transforms
from tqdm import tqdm

# 定义归一化方法
transform = transforms.Compose(
	# 首先装换数据为tensor张量
    [transforms.ToTensor(),
    # 对数据进行正态分布归一化，RGB三个通道每个通道均值为0.5，标准差为0.5
     transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))
    ]
    )

# 加载训练数据集
# root为数据存放的目录，train=Ture表示训练集，download=True表示要下载，transform为之前定义的归一化方法
trainset = torchvision.datasets.CIFAR10(root='./data', train=True,
                                       download=True, transform=transform)

# 定义数据集的加载方法， trainset为训练集， batch_size为单词训练的样本数，shuffle=True表示随机抽取样本，num_workers表示加载的线程数
trainloader = torch.utils.data.DataLoader(trainset, batch_size=16,
                                         shuffle=True, num_workers=2)

# 加载测试数据集
# root为数据存放的目录，train=False表示测试集，download=True表示要下载，transform为之前定义的归一化方法
testset = torchvision.datasets.CIFAR10(root='./data', train=False,
                                      download=True, transform=transform)

# 定义数据集的加载方法， testset为测试集， batch_size为单词训练的样本数，shuffle=False表示不随机抽取样本，num_workers表示加载的线程数
testloader = torch.utils.data.DataLoader(testset, batch_size=16,
                                        shuffle=False, num_workers=2)

```

1.2 私人数据集加载方法

- 在加载私人数据集时，数据集所在文件夹应保持以下结构：

```
/toy_dataset
  /class_1
  /class_2
  ...
```

- 程序上只需要做如下改动：

  privateset = torchvision.datasets.ImageFolder(root=image_path, train=True,
  download=True, transform=transform)

2. 定义神经网络

```
# 导入torch包
import torch
import torch.nn as nn
import torch.nn.functional as F

# 定义神经网络类
class Net(nn.Module):
	#定义神经网络结构, 输入数据 1x32x32
    def __init__(self):
        super(Net, self).__init__()
        # 第一层（卷积层）
        # 输入频道3， 输出频道6， 卷积3x3
        self.conv1 = nn.Conv2d(3,6,3)
        # 第二层（卷积层）
        # 输入频道6， 输出频道16， 卷积3x3
        self.conv2 = nn.Conv2d(6,16,3)
        # 第三层（全连接层）
        # 输入维度16x28x28=12544，输出维度 512
        self.fc1 = nn.Linear(16*28*28, 512)
        # 第四层（全连接层）
        # 输入维度512， 输出维度64
        self.fc2 = nn.Linear(512, 64)
        # 第五层（全连接层）
        # 输入维度64， 输出维度10
        self.fc3 = nn.Linear(64, 10)

    # 定义数据流向
    def forward(self, x):
    	# 数据先经过第一层卷积层
        x = self.conv1(x)
        # 经过激活函数
        x = F.relu(x)

        # 数据经过第二层卷积层
        x = self.conv2(x)
        # 经过激活函数
        x = F.relu(x)

        # 调整数据维度，‘-1’表示自动计算维度
        x = x.view(-1, 16*28*28)
        # 数据经过第三层全连接层
        x = self.fc1(x)
        # 数据经过激活函数
        x = F.relu(x)

         # 数据经过第四层全连接层
        x = self.fc2(x)
        # 数据经过激活函数
        x = F.relu(x)

        # 数据经过第五层全连接层，输出结果
        x = self.fc3(x)

        return x
```

3. 定义权值更新与损失函数

```
# 新建一个网络net
net = Net()

# 导入torch中优化器相关的包
import torch.optim as optim

# 定义损失函数为交叉熵函数
criterion = nn.CrossEntropyLoss()

# 优化器函数为随机梯度下降， 学习率为0。0001
optimizer = optim.SGD(net.parameters(), lr=0.0001, momentum=0.9)
```

4. 训练与测试神经网络

```
for epoch in range(2):
    #训练
    for i, data in enumerate(trainloader):
    	# 获得数据与标签
        images, labels = data
        # 得到网络的输出
        outputs = net(images)
        # 计算损失
        loss = criterion(outputs, labels)
        # 清零梯度
        optimizer.zero_grad()
        # 反向传播
        loss.backward()
        # 更新权重
        optimizer.step()
        # 计算总的损失
        running_loss += loss.item()
        #每1000 mini batch 测试一次
        if(i%1000 == 0):
            print('Epoch: %d,Step: %d, Loss: %.3f'%(epoch,i,loss.item()) )

```

- 可以发现，随着训练数据的增加，Loss 有明显减少的趋势。

5. 神经网络的保存与载入

- 保存模型：

  # 保存模型参数到路径'./model.pt'中

  torch.save(net.state_dict(), './model.pt')

- 读取模型:

  # 载入模型

  # 定义新 Net 类的网络 net_2

  net_2 = Net()

  # 将原参数路径的参数加载到 net_2 网络即可

  net_2.load_state_dict(torch.load('./model.pt'))

```
第一，需要在最开始import 加入如下代码，解决ssl问题
import ssl
ssl._create_default_https_context = ssl._create_unverified_context
第二
在import所有的结束后加入如下代码，解决多线程冻结问题，其余代码均应该在冒号后面整体移入
from tqdm import tqdm
if __name__ == '__main__':
第三 两重for循环中缺少定义running_loss
经过查证，这个应当定义在第二重for循环之前，定义如下
running_loss = 0.0
GoodLuck!
```
