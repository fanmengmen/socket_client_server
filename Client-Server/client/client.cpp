#include"EasyTcpClient.hpp"
#include"consistenceHash.hpp"
#include<vector>
#include<sstream>
using namespace std;

void randConnect()
{

    Login login;
    char name[] = {"guofan"};
    char password[] = {"123"};
    memcpy(&(login.userName), name, sizeof(name));
    memcpy(&(login.password), password, sizeof(password));
    srand(time(NULL));
    int port[3] = {3456,3457,3458};
    int count = 0;
    while(true)
    {
        EasyTcpClient client;
        int num = rand()%3;
        client.Connect("127.0.0.1", port[num]);
        client.SendData(&login);
        usleep(1);
        client.OnRun();   
        client.Close();
        usleep(1);
        count++;
        printf("count: %d\n", count);
    }
}



void hashConnect()
{

    int node_num = 3;
	int virtual_num = 20;

    int data_count = 100;
    int sample_count = 10;
	ConsistentHash* consistent_hash = new ConsistentHash(node_num, virtual_num);
	consistent_hash->Initialize();
	printf("consistent hash initialize success, node_num=%d, virtual_num=%d\n", node_num, virtual_num);
	
	vector<int> result(node_num,0);//节点存放数据数目统计
	vector<int> data_index(data_count,-1);//数据存放节点位置，下标是数据值i，它存放在data_index[i]上
	
	srand(time(NULL));
	for(int i=0; i<sample_count; ++i)
	{		
		int value = (rand()+getpid()) % data_count;
		stringstream ss;
		ss << value;
		const char* key = ss.str().c_str();
		size_t index = consistent_hash->GetServerIndex(key);
		result[index]++;
		if(data_index[value] < 0 || (int)index != data_index[value])
		{
			printf("key = %s, index = %lu\n", key, index);
		}
		data_index[value] = index;
	}
	
	int error_index = 3;
	consistent_hash->DeleteNode(error_index);
	printf("node error,index = %d\n", error_index);	
	for(int i=0; i<sample_count; ++i)
	{		
		int value = (rand()+getpid()) % data_count;
		stringstream ss;
		ss<<value;
		const char* key = ss.str().c_str();
		size_t index = consistent_hash->GetServerIndex(key);
		result[index]++;
		if(data_index[value] < 0 || (int)index != data_index[value])
		{
			printf("key = %s, index = %lu\n", key, index);
		}
		data_index[value] = index;
	}
	
	consistent_hash->AddNewNode(error_index);
	printf("node recover,index = %d\n", error_index);	
	for(int i=0; i<sample_count; ++i)
	{		
		int value = (rand()+getpid()) % data_count;
		stringstream ss;
		ss<<value;
		const char* key = ss.str().c_str();
		size_t index = consistent_hash->GetServerIndex(key);
		result[index]++;
		if(data_index[value] < 0 || (int)index != data_index[value])
		{
			printf("key = %s, index = %lu\n", key, index);
		}
		data_index[value] = index;
	}
	
	for(int i=0;i<node_num; ++i)
	{
		printf("index = %d, data_count = %d\n", i, result[i]);
	}
}



int main()
{

    Login login2;
    char name2[] = {"caozhangzhe"};
    char password2[] = {"1234"};
    memcpy(&(login2.userName), name2, sizeof(name2));
    memcpy(&(login2.password), password2, sizeof(password2));

    Login login3;
    char name3[] = {"liuyang"};
    char password3[] = {"12345"};
    memcpy(&(login3.userName), name3, sizeof(name3));
    memcpy(&(login3.password), password3, sizeof(password3));

    


    randConnect();


    // hashConnect();
   
    // while(client.isRun() || client2.isRun() || client1.isRun())
    // {
        // client.SendData(&login);
        // client2.SendData(&login2);
        // client1.SendData(&login3);
        // sleep(1);
        // client.OnRun();
        // client2.OnRun();
        // client1.OnRun();
        // sleep(1);
        
    // }
    return 0;
}