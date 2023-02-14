#

-仿真操作评分系统通过收集仿真管汇下放安装过程中的操作数据，设计和实现基于规则的仿真操作评分系统，并输出操作报告和评价。

- 本题目要求基于 C++技术、数据库技术、知识库等技术设计和开发一个能够对于仿真操作进行评价的系统。
- 要求实现：1 仿真操作过程数据收集入库；2 设计评价规则知识库 3 打分并输出评价报告。

- 要求： 1.有较强的自学能力，充足的时间。 2.熟练掌握 C++开发技术， 数据库技术。 3.较强的动手实验能力。

- 工作量： 3 - 4 个月

- 学生数量 1 人

- 检索、阅读有关虚拟仿真\知识库等相关的中英文文献约 10 篇，
  综述其研究、应用现状，翻译英文文献一篇。
  翻译的英文内容与毕业设计相关，字数 6000~10000 字

## 设计

1. 需求分析：首先，要建立一个数据库，用来存储仿真操作的相关的信息，如航空器的型号、操作的步骤等。其次，在知识库中设置专家系统，记录仿真操作过程中产生的正确或不正确的问答情况，并根据对应的操作情况进行给出正确答案。最后，根据仿真操作中实际产生的信息，与知识库中的相关信息进行比对，得出操作的正确率。

2. 技术实施：针对上述需求，设计者可使用 C++技术来开发程序，识别仿真操作的相关信息；可采用 MySQL 数据库，来存储仿真操作的记录；可通过对知识库的设计，让系统能够记录仿真操作的正确或不正确的答案，以及根据答案给出正确的答复。最后，利用比对技术，将实际产生的仿真操作信息与知识库中的信息进行比对，从而得出操作的正确率。
   （1）数据结构——存储数据；

3. 使用堆栈（stack）来存储仿真操作指令集；
4. 使用链表（list）来存储每一次仿真操作的输入和输出参数；
5. 使用中序遍历二叉树（binary tree）来存储仿真操作的分支选择；
6. 使用散列表（hash table）存储仿真操作的各种可能结果；

（2）核心算法——处理数据；

1. 递归地读取仿真操作指令集，根据情况有选择性地执行仿真操作，并将输入和输出参数保存到链表中；
2. 根据仿真操作的分支选择，递归地构建中序遍历的二叉树，并且进行实时更新；
3. 使用散列表将仿真各种可能的结果进行记录，以便以后参考；

（3）评价算法——人工模式或机器模式；

1. 人工模式：当仿真结束时，由设计者评价仿真操作的结果是否符合预期；
2. 机器模式：通过将仿真环境中的数据与准备好的标准数据相比对，评价仿真操作的结果是否符合预期。

## passage

本文介绍了一种基于仿真的操作评分系统的设计，实现了对操作人员的操作能力进行准确快速的评分。该系统使用硬件设备与软件及虚拟仿真环境相结合，提供一个综合的测试系统用于反映操作人员的操作能力。它以一种可以精确反映操作者操作能力的度量来取代传统的单一评分方法，如考试成绩。它利用智能传感器收集操作者操作行为的数据，并将其转换为操作者操作能力比较的可视化模型。此外，它还支持多种自定义的功能，如按时间和任务分类提供的报告，以及记录操作者的诊断信息以及针对特定技能的强化课程，以提高操作人员的技能。本文仅讨论仿真操作评分系统的设计，不涉及其具体实现。

1. 使用统计分析：可以使用统计学方法对评价规则知识库的数据进行分析，比如通过计算评价规则的准确率、召回率等指标来评估评价规则的性能。

2. 使用机器学习：可以使用机器学习算法来评估评价规则知识库的性能，比如可以建立一个多分类分类器，输入测试数据，然后预测结果，从而给出评价规则知识库的表现。

3. 使用可视化方法：可以使用可视化方法，比如画出评价规则知识库性能的折线图，比较不同规则在不同数据集上的正确率，从而发现时间段内规则的性能状况，为优化策略提供参考。
   仿真操作评分系统的具体实现主要包括以下几个步骤：

## j

1. 设计试验：根据实际系统的操作要求，搭建实验场景，确定各项试验项目及测试数据；

2. 设计评分系统：完成试验之后根据试验数据，对每一项试验项目进行评分，依据不同的评分标准以及实验要求可以设计出不同的评分系统；

3. 计算总得分：根据评分系统的设计，将每一项试验的得分进行综合，计算出总得分；

4. 数据保存：将每一次试验的所有数据进行保存，便于进行后续的分析比对；

5. 结果反馈：最后将试验的结果反馈给用户，可以是文字报告、图表展示等方式，让用户在最短的时间掌握试验结果。

## 1.功能要求

1. 能够进行实时的绩效评估，包括标准、结果、时间以及效率等；
2. 能够对操作流程进行设置，例如计算步骤、审核等；
3. 能够跟踪操作流程、绩效评估结果，生成定期报表；
4. 支持多人参与操作流程，并可以评估每个用户的绩效；
5. 能够检测不合规的操作，和记录操作日志。

## 2. 系统设计

1. 数据库设计：系统中使用 MySQL 数据库，数据库中有 7 张表，分别是 standards，results，time，efficiency，users，logs，checklist；
2. 前端界面设计：采用 Bootstrap 框架，设计一个可以实时显示操作流程绩效情况的表格，以及定义操作流程、检测不合规等功能；
3. 后台代码设计：系统中采用 PHP 编程语言开发，利用 Ajax 异步通信技术，连接前端和后台数据库，实现数据的更新，同时定时监测操作流程，如发现不合规的操作，就向用户发送提醒。

## 3. 实施方案

1. 建立数据库：根据系统功能，使用 MySQL 数据库建立 7 张表，分别是 standards，results，time，efficiency，users，logs，checklist；
2. 界面设计：使用 Bootstrap HTML、CSS、Javascript 等 Web 技术，设计界面，并能实时显示绩效情况，提供操作流程的定义，检测不合规的功能；
3. 后台编码：使用 PHP 编码技术，建立后台代码，与 MySQL 数据库连接，实现数据的更新，同时定时监测操作流程；
4. 测试：对系统功能进行功能测试，确保系统正常运行，满足用户需求；
5. 上线：将系统搭建在 Apache 服务器上，

```cpp
#include <iostream>
#include <map>

using namespace std;

int main()
{
map<string, int> ratingsMap;
ratingsMap["Excellent"] = 5;
ratingsMap["Good"] = 4;
ratingsMap["Average"] = 3;
ratingsMap["Poor"] = 2;
ratingsMap["Very Poor"] = 1;

    string rating;
    cout << "Enter rating : ";
    cin >> rating;

    // Checking if rating is present in map
    if (ratingsMap.find(rating) != ratingsMap.end()) {
        cout << ratingsMap[rating];
    }
    else {
        cout << "Invalid Rating";
    }

    return 0;

}
```

### 摘要

本文介绍了一种基于仿真的操作评分系统的设计。首先，介绍了仿真操作评分系统及其工作原理，以及它在衡量操作技能方面的优势。其次，根据操作技能指标，设计出基于可视化仿真的操作评分系统，并从测量模型、数据采集过程及评分结果计算等三个方面详细介绍系统的实施步骤。最后，指出基于仿真的操作评分系统的优势，重申了它在衡量操作技能时的重要作用。

关键词：仿真；操作评分；可视化

1. Introduction

Nowadays, operation competency is increasingly valued in many industrial production and service enterprises. As an important way to measure and evaluate the operation skills of operators, operational scoring systems have been widely used in enterprises. Operation scoring systems are mainly composed of evaluation indicators, scoring methods and scoring results. At present, the traditional operation scoring system is mainly composed of paper assessment, manual manual and questionnaire survey. However, these methods can only provide a subjective and qualitative assessment of the operator's operation level and do not meet the demand of the enterprise for timely and objective evaluation. In view of this situation, this paper proposes a design of operation scoring system based on simulation.

2. Simulated Operational Scoring System

Simulated operational scoring systems are commonly used in businesses to assess the skills of their employees. The system uses a simulated environment to assess the operator's performance according to specified criteria. In particular, it can effectively assess the actual operational skills of the operator and provide timely feedback about the operator's operation performance. The evaluation scores obtained by the operator can be used to assess their operation competency and even predict their operation level.

The simulated operational scoring system consists of three main components: visualization simulator, data collection process and score calculation. The visualization simulator is used to reflect the relevant operating environment and the operator's operations. The data collection process is used to record the operator's operational behavior and provides the necessary data for scoring. The score calculation part is used to calculate the final score according to the set criteria.

3. System Design and Implementation

Based on the above characteristics of the simulated operational scoring system, this paper puts forward the design of a visualized simulated operational scoring system. The system design and implementation are divided into three parts: measurement model, data collection process and score calculation.

(1) Measurement Model. The system adopts a criteria-based evaluation model in which the evaluation metrics of the operator's operation performance are set based on the tasks performed. These metrics are used to measure the operator's actual operating experience and evaluate his ability.

(2) Data Collection Process. In order to collect the relevant data of the operator, the system uses a combination of manual observation and automated data collection. During the process, the behavior of the operator is recorded and analyzed. At the same time, the system also collects the data of the operator's operation status, such as speed and accuracy.

(3) Score Calculation. Finally, the system calculates the score based on the data collected in the previous step. The score is calculated according to the preset evaluation criteria.

4. Advantages

The simulated operational scoring system has several advantages over traditional methods. First, it can accurately quantify the operator's operational competence, thus providing more objective assessments of the operator's abilities. Second, the system can provide timely feedback to the operator, allowing them to make necessary adjustments in their performance. Finally, the system can be used to compare the performance of multiple operators, which can help managers make better decisions.

5. Conclusion

In conclusion, this paper has introduced the design of a simulated operational scoring system. The system takes advantage of a visualized simulator, data collection process and score calculation to accurately assess the operational competence of operators. This system has several advantages compared with traditional methods, making it an effective tool for assessing the skills of operators.
