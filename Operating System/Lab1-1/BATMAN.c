#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define MAX 100										// 最大车辆数
typedef enum directionT {South, East, North, West} direction;						// 方向枚举
typedef struct CarT										// 车辆结构体
{	
	int id;										// 车辆代号 
	direction dir;									// 车辆代表的方向
} Car;

// 四个方向的出发信号
pthread_cond_t firstNorth;
pthread_cond_t firstEast;
pthread_cond_t firstSouth;
pthread_cond_t firstWest;

// 四个方向的路口资源锁 和出发信号对应
pthread_mutex_t source_north;
pthread_mutex_t source_east;
pthread_mutex_t source_south;
pthread_mutex_t source_west;

// 四个方向的进入准备出发位置的信号
pthread_cond_t queueNorth;
pthread_cond_t queueEast;
pthread_cond_t queueSouth;
pthread_cond_t queueWest;

// 四个方向准备通行锁 和准备出发信号对应
pthread_mutex_t crossingNorth;
pthread_mutex_t crossingEast;
pthread_mutex_t crossingSouth;
pthread_mutex_t crossingWest;

// 死锁与死锁信号
pthread_cond_t deadlock;
pthread_mutex_t wait_deadlock;

// 四个方向的车辆数统计
int count_North = 0;
int count_East = 0;
int count_South = 0;
int count_West = 0;

void *check_deadlock(void* c)									// 死锁判断线程
{
	while(1)
	{
		pthread_mutex_lock(&wait_deadlock);						// 锁定
		pthread_cond_wait(&deadlock, &wait_deadlock);					// 等待死锁发生
		
		if(count_South > 0 && count_East > 0 && count_North > 0 && count_West > 0)		// 各方向都有车则说明死锁
		{
			printf("DEADLOCK: car jam detected, signalling North to go.\n");		// 打印死锁提示
			usleep(1);
			pthread_cond_signal(&firstNorth);					// 让北边的车辆先行
		}
		
		pthread_mutex_unlock(&wait_deadlock);						// 解锁
	}
}

// 注释北边车辆函数 其他方向同理
void *car_from_north(void *c)
{
	pthread_mutex_lock(&crossingNorth);							// 锁定该方向准备通行
	Car *car = (Car*)c;									// 传入参数类型转换
	pthread_cond_wait(&queueNorth, &crossingNorth);					// 等待通行信号
	count_North += 1;									// 更新车辆数目统计
	printf("car %d from north arrives crossing\n", car->id);					// 打印到达提示
	usleep(1);
	pthread_mutex_unlock(&crossingNorth);							// 解除该方向准备通行锁定 开始通行
	pthread_mutex_lock(&source_north);							// 锁定该方向通行资源锁
	if (count_West > 0)
	{
		pthread_cond_signal(&deadlock);						// 如果右边有车 则进行死锁判断
		pthread_cond_wait(&firstNorth, &source_north);					// 如果右边有车 等待右边车通过后唤醒本车
	}
	count_North--;									// 更新车辆数目统计
	printf("car %d from north leaving crossing\n", car->id);					// 打印离开提示
	pthread_mutex_unlock(&source_north);							// 解除该方向通行资源锁 离开路口
	pthread_cond_signal(&firstEast);							// 发送信号唤醒 左边的车（如果有）
	usleep(1);
	pthread_cond_signal(&queueNorth);							// 唤醒同方向队列的下一辆车
	pthread_exit(NULL);									// 离开线程
}

void *car_from_east(void *c)
{
	pthread_mutex_lock(&crossingEast);
	Car *car = (Car*)c;
	pthread_cond_wait(&queueEast, &crossingEast);
	count_East += 1;
	printf("car %d from east arrives crossing\n", car->id);
	usleep(1);
	pthread_mutex_unlock(&crossingEast);
	pthread_mutex_lock(&source_east);
	if (count_North > 0)
	{
		pthread_cond_signal(&deadlock);
		pthread_cond_wait(&firstEast, &source_east);
	}
	count_East--;
	printf("car %d from east leaving crossing\n", car->id);
	pthread_mutex_unlock(&source_east);
	pthread_cond_signal(&firstSouth);
	usleep(1);
	pthread_cond_signal(&queueEast);
	pthread_exit(NULL);
}

void *car_from_south(void *c)
{
	pthread_mutex_lock(&crossingSouth);
	Car *car = (Car*)c;
	pthread_cond_wait(&queueSouth, &crossingSouth);
	count_South += 1;
	printf("car %d from south arrives crossing.\n", car->id);
	usleep(1);
	pthread_mutex_unlock(&crossingSouth);
	pthread_mutex_lock(&source_south);
	if (count_East > 0)
	{
		pthread_cond_signal(&deadlock);
		pthread_cond_wait(&firstSouth, &source_south);
	}
	count_South--;
	printf("car %d from south leaving crossing.\n", car->id);
	pthread_mutex_unlock(&source_south);
	pthread_cond_signal(&firstWest);
	usleep(1);
	pthread_cond_signal(&queueSouth);
	pthread_exit(NULL);
}

void *car_from_west(void *c)
{
	pthread_mutex_lock(&crossingWest);
	Car *car = (Car*)c;
	pthread_cond_wait(&queueWest, &crossingWest);
	count_West += 1;
	printf("car %d from west arrives crossing\n", car->id);
	usleep(1);
	pthread_mutex_unlock(&crossingWest);
	pthread_mutex_lock(&source_west);
	if (count_South > 0)
	{
		pthread_cond_signal(&deadlock);
		pthread_cond_wait(&firstWest, &source_west);
	}
	count_West--;
	printf("car %d from west leaving crossing\n", car->id);
	pthread_mutex_unlock(&source_west);
	pthread_cond_signal(&firstNorth);
	usleep(1);
	pthread_cond_signal(&queueWest);
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	// 初始化
	pthread_mutex_init(&source_north, NULL);
	pthread_mutex_init(&source_east, NULL);
	pthread_mutex_init(&source_south, NULL);
	pthread_mutex_init(&source_west, NULL);

	pthread_mutex_init(&crossingNorth, NULL);
	pthread_mutex_init(&crossingEast, NULL);
	pthread_mutex_init(&crossingSouth, NULL);
	pthread_mutex_init(&crossingWest, NULL);

	pthread_cond_init(&firstNorth, NULL);
	pthread_cond_init(&firstEast, NULL);
	pthread_cond_init(&firstSouth, NULL);
	pthread_cond_init(&firstWest, NULL);

	pthread_cond_init(&queueNorth, NULL);
	pthread_cond_init(&queueEast, NULL);
	pthread_cond_init(&queueSouth, NULL);
	pthread_cond_init(&queueWest, NULL);
	
	pthread_mutex_init(&wait_deadlock, NULL);
	pthread_cond_init(&deadlock, NULL);
	
	Car cars[MAX];
	pthread_t car_threads[MAX];

	pthread_t check;
	pthread_create(&check, NULL, check_deadlock, NULL);					// 创建死锁检测线程
	
	for (int i = 0; i < strlen(argv[1]); i++)							// 遍历输入字符串并创建对应线程
	{
		cars[i].id = i + 1;
		switch (argv[1][i]) 
		{
			case 'n':
				cars[i].dir = North;
				pthread_create(&car_threads[i], NULL, car_from_north, (void*)&cars[i]);
				break;
			case 'e':
				cars[i].dir = East;
				pthread_create(&car_threads[i], NULL, car_from_east, (void*)&cars[i]);
				break;
			case 's':
				cars[i].dir = South;
				pthread_create(&car_threads[i], NULL, car_from_south, (void *)&cars[i]);
				break;
			case 'w':
				cars[i].dir = West;
				pthread_create(&car_threads[i], NULL, car_from_west, (void*)&cars[i]);
				break;
			default:
				printf("Wrong Input!\n");
				break;
		}
		usleep(1);
	}
	usleep(1);

	// 提供初始信号
	pthread_cond_signal(&queueNorth);
	pthread_cond_signal(&queueEast);	
	pthread_cond_signal(&queueSouth);	
	pthread_cond_signal(&queueWest);	
	
	for (int i = 0; i < strlen(argv[1]); i++) 
		pthread_join(car_threads[i], NULL);						// 等待全部车辆线程结束
}