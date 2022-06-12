# install pytorch

- 有两个版本，一个 CPU，一个 cuda

- in ArchLinux: sudo python-pytorch (python-pytorch-cuda) 即可
- 可阅读官方文档：`https://pytorch.org/get-started/locally/`

# gpu + cuda + cudnn

- sudo pacman -S nvidia nvidia-settings opencl-nvidia cuda cudnn
- 可选：cuda-gdb (需要 ncurses5-compat-libs)

- 安装参见：https://wiki.archlinux.org/title/GPGPU#CUDA

- GPU（图像处理器，Graphics Processing Unit）和 CPU（中央处理器，Central Processing Unit）在设计上的主要差异在于 GPU 有更多的运算单元（如图中绿色的 ALU），而 Control 和 Cache 单元不如 CPU 多，这是因为 GPU 在进行并行计算的时候每个运算单元都是执行相同的程序，而不需要太多的控制。Cache 单元是用来做数据缓存的，CPU 可以通过 Cache 来减少存取主内存的次数，也就是减少内存延迟（memory latency）。GPU 中 Cache 很小或者没有，因为 GPU 可以通过并行计算的方式来减少内存延迟。因此 CPU 的 Cahce 设计主要是实现低延迟，Control 主要是通用性，复杂的逻辑控制单元可以保证 CPU 高效分发任务和指令。所以 CPU 擅长逻辑控制，是串行计算，而 GPU 擅长高强度计算，是并行计算。打个比方，GPU 就像成千上万的苦力，每个人干的都是类似的苦力活，相互之间没有依赖，都是独立的，简单的人多力量大；CPU 就像包工头，虽然也能干苦力的活，但是人少，所以一般负责任务分配，人员调度等工作。

- 可以看出 GPU 加速是通过大量线程并行实现的，因此对于不能高度并行化的工作而言，GPU 就没什么效果了。而 CPU 则是串行操作，需要很强的通用性，主要起到统管和分配任务的作用。

- CUDA 的官方文档（参考资料 1）是这么介绍 CUDA 的：a general purpose parallel computing platform and programming model that leverages the parallel compute engine in NVIDIA GPUs to solve many complex computational problems in a more efficient way than on a CPU.
  换句话说 CUDA 是 NVIDIA 推出的用于自家 GPU 的并行计算框架，也就是说 CUDA 只能在 NVIDIA 的 GPU 上运行，而且只有当要解决的计算问题是可以大量并行计算的时候才能发挥 CUDA 的作用。

- 在 CUDA 的架构下，一个程序分为两个部份：host 端和 device 端。Host 端是指在 CPU 上执行的部份，而 device 端则是在显示芯片上执行的部份。Device 端的程序又称为 “kernel”。通常 host 端程序会将数据准备好后，复制到显卡的内存中，再由显示芯片执行 device 端程序，完成后再由 host 端程序将结果从显卡的内存中取回。

- 在 CUDA 架构下，显示芯片执行时的最小单位是 thread。数个 thread 可以组成一个 block。一个 block 中的 thread 能存取同一块共享的内存，而且可以快速进行同步的动作。每一个 block 所能包含的 thread 数目是有限的。不过，执行相同程序的 block，可以组成 grid。不同 block 中的 thread 无法存取同一个共享的内存，因此无法直接互通或进行同步。因此，不同 block 中的 thread 能合作的程度是比较低的。不过，利用这个模式，可以让程序不用担心显示芯片实际上能同时执行的 thread 数目限制。例如，一个具有很少量执行单元的显示芯片，可能会把各个 block 中的 thread 顺序执行，而非同时执行。不同的 grid 则可以执行不同的程序（即 kernel）。

- cuDNN（CUDA Deep Neural Network library）：是 NVIDIA 打造的针对深度神经网络的加速库，是一个用于深层神经网络的 GPU 加速库。如果你要用 GPU 训练模型，cuDNN 不是必须的，但是一般会采用这个加速库。
- NVIDIA CUDA® 深度神经网络库(cuDNN) 是经 GPU 加速的深度神经网络基元库。 cuDNN 可大幅优化标准例程（例如用于前向传播和反向传播的卷积层、池化层、归一化层和激活层）的实施。 世界各地的深度学习研究人员和框架开发者都依赖 cuDNN 实现高性能 GPU 加速。--https://developer.nvidia.com/zh-cn/cudnn

# matplotlib

- pip install matplotlib

# torchvision

- pip install torchvision
