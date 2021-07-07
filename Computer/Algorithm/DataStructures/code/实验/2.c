#include <stdio.h>

typedef struct teacher_info  
{
	int key;
	int away;
	int back;
}Teacher;
 
int main()
{
	int i, j, keys_num, teachers_num;
	scanf("%d%d", &keys_num, &teachers_num);
	int Away[teachers_num][2], Back[teachers_num][2]; //这两个数组的第一维是钥匙编号，第二维是时间点（时刻）
	int box[keys_num + 1], key_pos[keys_num + 1]; //box是盒子摆放的钥匙，key_pos是每个钥匙当前的位置
	
	for(i=1; i<=keys_num; i++)
	{
		box[i] = i;
		key_pos[i] = i;
	}
	
	for(i=0; i<teachers_num; i++)    //初始化两个二维数组的第一位，把钥匙编号置为0 
	{
		Away[i][0] = 0;
		Back[i][0] = 0;
	}
	
	Teacher input[teachers_num];
	int time_count = 0;
	for(i=0; i<teachers_num; i++)    //输入 
	{
		scanf("%d%d%d", &input[i].key, &input[i].away, &input[i].back);
		Back[i][0] = Away[i][0] = input[i].key;
		Away[i][1] = input[i].away;
		Back[i][1] = input[i].away + input[i].back; 
	}
	
	//对Away和Back排序
	int temp0, temp1;
	for(i=0; i<teachers_num-1; i++)
	{
		for(j=0; j<teachers_num-i-1; j++)
		{
			if(Away[j][1] > Away[j+1][1])
			{
				temp0 = Away[j][0];
				temp1 = Away[j][1];
				Away[j][0] = Away[j+1][0];
				Away[j][1] = Away[j+1][1];
				Away[j+1][0] = temp0;
				Away[j+1][1] = temp1;
			}
			
			if(Back[j][1] > Back[j+1][1])
			{
				temp0 = Back[j][0];
				temp1 = Back[j][1];
				Back[j][0] = Back[j+1][0];
				Back[j][1] = Back[j+1][1];
				Back[j+1][0] = temp0;
				Back[j+1][1] = temp1;
			}
			else if(Back[j][1] == Back[j+1][1] && Back[j][0] > Back[j+1][0])//如果时间相同，按钥匙字典序还 
			{
				temp0 = Back[j][0];
				temp1 = Back[j][1];
				Back[j][0] = Back[j+1][0];
				Back[j][1] = Back[j+1][1];
				Back[j+1][0] = temp0;
				Back[j+1][1] = temp1;
			}
		}
	}
	
	int start = Away[0][1], end = Back[teachers_num - 1][1];
	int t = start;                                 //时间
	int away_count = 0;
	int back_count = 0;
	while(t <= end)
	{
		while(t == Back[back_count][1])       //还钥匙 
		{
			for(j=1; j<=keys_num; j++)        //找空位 
			{
				if(!box[j])
					break;
			}
			box[j] = Back[back_count][0];
			key_pos[box[j]] = j;
			back_count ++; 
		}
		
		while(t == Away[away_count][1])          //拿钥匙 
		{
			int temp_pos = key_pos[Away[away_count][0]];
			box[temp_pos] = 0;
			away_count ++;
		}
		
		t ++;
	}
	
	for(i=1; i<=keys_num; i++)                 //输出 
	{
		printf("%d ", box[i]);
	}
	
	return 0;
}

/*
5 7
1 1 14
3 3 12
1 15 12
2 7 20
3 18 12
4 21 19
5 30 9
*/

