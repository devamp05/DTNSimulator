/*
 * group-10 
 * Narek Tamrazyan NSID: uwo916 Student Number: 11315451
 * Devam Punitbhai Patel NSID: dns682 Student Number: 11316715
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <list.h>
#include <pthread.h>
#include <semaphore.h> 

int N;	/*total nodes in the system*/
int K;	/*total timeSteps to run the simulator for*/	
int D;	/*total destination nodes*/	
int R;	/*transmission range allowed*/	
int Q;	/*buffer space*/	
int P;	/*percentage space every leavs for itself*/	

/*
#define N 11	
#define K 2		
#define D 1		
#define R 200	
#define Q 10	
#define P 20	
*/
#define STARTING_PORT 37000
int NO;

int whosTurn, accepting, whosSending, waiting, totalWaiting, whosTransmiting;
int totalHops, totalPacketsReceived, flag, loopCounter;

pthread_mutex_t whosSending_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t totalWaiting_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t whosTransmiting_mutex = PTHREAD_MUTEX_INITIALIZER;

sem_t *mutexs;
int *mutexsCheck;

typedef struct coordinates
{
	int x;
	int y;
}coordinates;

typedef struct dataPacket
{
	int id;
	void *data;
	int tempStepsAfterGeneration;
	int *deleted;
}dataPacket;

void *threadFunction2(void *id)
{
    int i, j;
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; /* connector's address information*/
    socklen_t sin_size;
    int yes=1;
    int rv;
	char port[12];
	int *sendingSockets, *receivingSockets;
	coordinates myLocation;
	coordinates *map;
	LIST *myBuffer = ListCreate();
	dataPacket *myPacket;
	
	sendingSockets = malloc(sizeof(int) * N);
	receivingSockets = malloc(sizeof(int) * N);

	map = malloc(sizeof(coordinates) * N);
	for(j = 0; j < N; j++)
	{
		receivingSockets[j] = -1;
	}

	while(1)
	{
		mutexsCheck[*(int *)id] = 1;
		sem_wait(&mutexs[*(int *)id]);
		mutexsCheck[*(int *)id] = 0;
		if(whosTurn == *(int *)id)
		{
			/*if it is my turn to accept connections*/
    		int sockfd;
	    	memset(&hints, 0, sizeof hints);
			hints.ai_family = AF_INET;	/*using IPv4*/
    		hints.ai_socktype = SOCK_STREAM;
	    	hints.ai_flags = AI_PASSIVE; /* use my IP*/

			sprintf(port, "%d", STARTING_PORT + whosTurn);
		    if ((rv = getaddrinfo(NULL, port
			, &hints, &servinfo)) != 0) {
        		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		    }

			/* loop through all the results and bind to the first we can*/
    		for(p = servinfo; p != NULL; p = p->ai_next) {
    	    	if ((sockfd = socket(p->ai_family, p->ai_socktype,
        	    	p->ai_protocol)) == -1) {
	            	perror("server: socket");
			        continue;
    			}

        		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
            		sizeof(int)) == -1) {
		        	perror("setsockopt");
    		        exit(1);
	        	}

			    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
    			    close(sockfd);
					printf("myId: %d\n", *(int *)id);
	    		    perror("server: bind");
            		continue;
		        }

			    break;
    		}

		    freeaddrinfo(servinfo); /* all done with this structure*/

	    	if (p == NULL)  {
    	    	fprintf(stderr, "server: failed to bind\n");
	    	    exit(1);
	    	}

			if (listen(sockfd, N) == -1) {
    			perror("listen");
        		exit(1);
		    }
			for(j = 0; j < N; j++)
			{
				/*accepting[j] = 1;*/
				sendingSockets[j] = -1;
			}
			for(i = 0; i < N; i++)
			{
				int new_fd;
				if(i == *(int *)id)
				{
					continue;
				}
				if(!mutexsCheck[i])
				{
					printf("ERROR wrong ordering while accepting "
					"connections");
				}
				sem_post(&mutexs[i]);
	        	new_fd = accept(sockfd, (struct sockaddr *)&their_addr, 
				&sin_size);
		    	if (new_fd == -1) 
				{
    		    	perror("accept");
        		    continue;
	        	}
				sendingSockets[i] = new_fd;
			}
			whosTurn++;
			if(whosTurn == N)
			{
				break;	
			}
			sem_post(&mutexs[whosTurn]);
		}
		else
		{
			int sockfd, temp;
			if(whosTurn == N)
			{
				break;
			}
			temp = whosTurn;
			/*it means we are sending connection*/
	    	memset(&hints, 0, sizeof hints);
		    hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;

			sprintf(port, "%d", STARTING_PORT + whosTurn);

	    	if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
	        	fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
			    exit(1);
    		}

			/* loop through all the results and connect to the first 
			* we can*/
		    for(p = servinfo; p != NULL; p = p->ai_next) {
    		    if ((sockfd = socket(p->ai_family, p->ai_socktype,
        		    	p->ai_protocol)) == -1) {
	        		perror("client: socket");
    	        	continue;
		        }

			    if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
    			    close(sockfd);
        			perror("client: connect");
            		continue;
			    }

		     	break;
			}

    		if (p == NULL) {
        		fprintf(stderr, "client: failed to connect\n");
			    exit(1);
    		}
			receivingSockets[temp] = sockfd;
		}
	}
	if(*(int *)id == N-1)
	{
		whosSending = 0;
		totalWaiting = 0;
		for(i = 0; i < N-1; i++)
		{
			if(!mutexsCheck[i])
			{
				printf("ERROR something wrong with the ordering for id: "
				"%d when releasing them after making connection\n", i);
			}
		}
		sem_post(&mutexs[0]);
		mutexsCheck[*(int *)id] = 1;
		sem_wait(&mutexs[*(int *)id]);
	}

	myLocation.x = rand() % 1000;
	myLocation.y = rand() % 1000;

	printf("id: %d startLocation x, y: %d, %d\n", *(int *)id, 
	myLocation.x, myLocation.y);


	while(1)
	{
        pthread_mutex_lock(&whosSending_mutex);
		if(whosSending == *(int *)id)
		{
			pthread_mutex_unlock(&whosSending_mutex);
			for(i = 0; i < N; i++)
			{
				if(i == *(int *)id)
				{
					continue;
				}
				if(totalWaiting == N-1)
				{
					break;
				}
				if(!mutexsCheck[i])
				{
					printf("id: %d was not waiting\n", i);
				}
				mutexsCheck[i] = 0;

				sem_post(&mutexs[i]);
			}
			while(totalWaiting != N-1)
			{
				/*let them all be there*/
				sleep(1);
			}
            pthread_mutex_lock(&totalWaiting_mutex);

			/*they all should be waiting now*/
			totalWaiting = 0;
            
			pthread_mutex_unlock(&totalWaiting_mutex);

			for(i = 0; i < N; i++)
			{
				int sendSock;

				if(i == *(int *)id)
				{
					continue;
				}
				sendSock = sendingSockets[i];
				if (send(sendSock, &myLocation, sizeof(coordinates), 0) 
				== -1)
    	    		perror("send");
			}
			while(totalWaiting != N-1)
			{
				/*let them all wait*/
				sleep(1);
			}
        	pthread_mutex_lock(&whosSending_mutex);

			whosSending++;
			if(whosSending == N)
			{
				pthread_mutex_unlock(&whosSending_mutex);

				break;
			}

			pthread_mutex_unlock(&whosSending_mutex);

			pthread_mutex_lock(&totalWaiting_mutex);

			totalWaiting = 0;

            pthread_mutex_unlock(&totalWaiting_mutex);

			sem_post(&mutexs[whosSending]);
			mutexsCheck[*(int *)id] = 1;
			sem_wait(&mutexs[*(int *)id]);
		}
		else
		{
			int recvSock, numB;
				
			if(whosSending == N)
			{
				pthread_mutex_unlock(&whosSending_mutex);

				break;
			}
			pthread_mutex_unlock(&whosSending_mutex);
				
			recvSock = receivingSockets[whosSending];

			pthread_mutex_lock(&totalWaiting_mutex);

			totalWaiting++;

			pthread_mutex_unlock(&totalWaiting_mutex);

			if((numB = recv(recvSock, &map[whosSending], 
			sizeof(coordinates), 0)) == -1)
			{
				perror("recv");
				exit(1);
			}
			pthread_mutex_lock(&totalWaiting_mutex);

			totalWaiting++;
			
			pthread_mutex_unlock(&totalWaiting_mutex);
            

			/*now we shall wait for others to complete*/
			mutexsCheck[*(int *)id] = 1;
			sem_wait(&mutexs[*(int *)id]);
			mutexsCheck[*(int *)id] = 0;
		}
	}

	if(*(int *)id == N-1)
	{
		for(i = 0; i < N-1; i++)
		{
			if(!mutexsCheck[i])
			{
				printf("ERROR something wrong with the ordering for id: "
				"%d here\n", i);
			}
		}
		flag = 1;
		loopCounter = 0;
		whosTransmiting = N-1;
	}
	while(flag)
	{
		if(ListCount(myBuffer) < Q)
		{
			myPacket = malloc(sizeof(dataPacket) * 1);
			myPacket->id = *(int *)id;
			myPacket->tempStepsAfterGeneration = 0;
			myPacket->deleted = &NO;

			ListAppend(myBuffer, myPacket);
		}
		else
		{
			printf("%d discarded packet because queue full\n", *(int *)id);
		}

		while(whosTransmiting > D-1)
		{
        	pthread_mutex_lock(&whosTransmiting_mutex);
			if(whosTransmiting == *(int *)id)
			{
			/*
				printf("%d running when loopCounter is: %d\n", *(int *)id, 
				loopCounter);
				*/
				LIST *temp;	/*a temporary list to hold all the options I have
				within transmission range*/
				/*printf("%dwas here\n", *(int *)id);*/
				pthread_mutex_unlock(&whosTransmiting_mutex);
			/*now we will see which nodes are within my transmission range
			 * starting from the begining because the first D nodes are the
			 * destinations and we want to reach a destination first if 
			 * possible*/

				temp = ListCreate();

				for(i = 0; i < *(int *)id; i++)
				{
					if(map[i].x <= myLocation.x + R && map[i].x >= 
					myLocation.x)
					{
						if(map[i].y <= myLocation.y + R && map[i].y >= 
						myLocation.y)
						{
							int *index;
							index = malloc(sizeof(int) * 1);
							*index = i;
							ListAppend(temp, index);
						}
						else if(map[i].y >= myLocation.y - R && map[i].y <=
						myLocation.y)
						{
							int *index;
							index = malloc(sizeof(int) * 1);
							*index = i;
							ListAppend(temp, index);	
						}
					}
					else if(map[i].x >= myLocation.x - R && map[i].x <= 
					myLocation.x)
					{
						if(map[i].y <= myLocation.y + R && map[i].y >= 
						myLocation.y)
						{
							int *index;
							index = malloc(sizeof(int) * 1);
							*index = i;
							ListAppend(temp, index);	
						}
						else if(map[i].y >= myLocation.y - R && map[i].y <=
						myLocation.y)
						{
							int *index;
							index = malloc(sizeof(int) * 1);
							*index = i;
							ListAppend(temp, index);
						}
					}
				}
				/*printf("%d has %d in range\n", *(int *)id, 
				 * ListCount(temp));*/
				while(ListCount(temp) > 0)
				{
					int *index, recvSock, recvAck, numPackets, sendSock, numB;

					ListFirst(temp);

					index = ListRemove(temp);

				/*talk to index according to the protocol if they accept then
				 * discard other nodes in the list*/
				/*printf("comes here\n");*/	/*it came here for n = 11 so 
				strategy is working until this point it is just due to the 
				randomness that sometimes it wouldn't even come to this point
				where it has other nodes in transmission range*/

					if(!mutexsCheck[*index])
					{
					/*
					while(!mutexsCheck[*index])
					{
						sleep(1);
					}
					*/
						printf("ERROR %d should be waiting before "
						"transmission but it is not\n", *index);
					}
					
					numPackets = ListCount(myBuffer);

					/*printf("numPackets: %d\n", numPackets);*/
					if(numPackets > 0)
					{

						mutexsCheck[*index] = 0;

						sem_post(&mutexs[*index]);


				/*first we receive an ack that they are ready for receiving*/
						recvSock = receivingSockets[*index];
						sendSock = sendingSockets[*index];

						if((numB = recv(recvSock, &recvAck, 
						sizeof(int), 0)) == -1)
						{
							perror("recv");
							exit(1);
						}

						if (send(sendSock, &numPackets, sizeof(int), 0) == -1)
    	    				perror("send");

				/*if its a destination then we just send all the packets we
				 * have*/
						if(*index <= D-1)
						{
					/*if its a destination then we just send all our packets*/
							while(ListCount(myBuffer) > 0)
							{
								dataPacket sendPacket;

								ListFirst(myBuffer);
	
								myPacket = ListRemove(myBuffer);

								myPacket->tempStepsAfterGeneration++;

								sendPacket.id = myPacket->id;

								sendPacket.tempStepsAfterGeneration = 
								myPacket->tempStepsAfterGeneration;

								sendPacket.id = htons(sendPacket.id);

								sendPacket.tempStepsAfterGeneration = 
								htons(sendPacket.tempStepsAfterGeneration);

								if(send(sendSock, &sendPacket, 
								sizeof(dataPacket),0) == -1)
									perror("send");

								free(myPacket);
							}
				
							if((numB = recv(recvSock,&recvAck, 
							sizeof(int), 0)) == -1)
							{
								perror("recv");
								exit(1);
							}

					/*assert*/
							if(recvAck != numPackets)
							{
								printf("Some packets got lost in "
								"transmission "
								"from %d to destination: %d\n", *(int *)id, 
								*index);
							}

					/*wait for receiver to go back to waiting*/
							while(!mutexsCheck[*index])
							{
								printf("waiting from here\n");
								sleep(1);
							}
						}
						else
						{
							int numSent;
					/*now we send how much data we have*/

					/*if its not a destination node then we have to see if it
					 * can accept our packets and if it can how much of them*/
							if((numB = recv(recvSock,&recvAck, 
							sizeof(int), 0)) == -1)
							{
								perror("recv");
								exit(1);
							}

							numSent = 0;

							while(numSent < recvAck)
							{
								dataPacket sendPacket;
	
								ListFirst(myBuffer);

								myPacket = ListRemove(myBuffer);

								myPacket->tempStepsAfterGeneration++;
						
								sendPacket.id = myPacket->id;

								sendPacket.tempStepsAfterGeneration =
								myPacket->tempStepsAfterGeneration;

								sendPacket.id = htons(sendPacket.id);

								sendPacket.tempStepsAfterGeneration = 
								htons(sendPacket.tempStepsAfterGeneration);

								if(send(sendSock, &sendPacket, 
								sizeof(dataPacket),
								0) == -1)
									perror("send");
			
								ListAppend(myBuffer, myPacket);

								numSent++;
							}
					
							if(numSent > 0)
							{
								if((numB = recv(recvSock,&recvAck, 
								sizeof(int), 0)) == -1)
								{
									perror("recv");
									exit(1);
								}
						
						/*assert*/
								if(recvAck != numSent)
								{
							printf("Some packets got lost in transmission "
							"from %d to destination: %d sent: %d, theyGot: "
							"%d\n", *(int *)id, *index, numSent, recvAck);
								}
							}
					
					/*wait for receiver to go back to waiting*/
							while(!mutexsCheck[*index])
							{
						/*printf("is it here?\n");*/
								sleep(1);
							}
					/*printf("got out\n");*/
						}
					}
        		}
				/*
				while(!mutexsCheck[*index])
				{	
					sleep(1);
				}
				*/

				pthread_mutex_lock(&whosTransmiting_mutex);

				whosTransmiting--;
				if(whosTransmiting > D-1)
				{
					sem_post(&mutexs[whosTransmiting]);
				}
				else
				{
        			pthread_mutex_unlock(&whosTransmiting_mutex);
					break;
				}

        		pthread_mutex_unlock(&whosTransmiting_mutex);
			
				mutexsCheck[*(int *)id] = 1;

				sem_wait(&mutexs[*(int *)id]);
			}
			else
			{
				int ack, numPackets, sendSock, recvSock, numB;

				if(whosTransmiting < D)
				{
					break;
				}
				
			
				ack = 1;

				sendSock = sendingSockets[whosTransmiting];

				recvSock = receivingSockets[whosTransmiting];
			
			/*printf("%dwas here 2\n", *(int *)id);*/
        		
				pthread_mutex_unlock(&whosTransmiting_mutex);

				if (send(sendSock, &ack, sizeof(int), 0) == -1)
    	    		perror("send");
			
				if((numB = recv(recvSock, &numPackets, sizeof(int), 0)) == -1)
				{
					perror("recv");
					exit(1);
				}

				/*
				if(numPackets > 0)
				{
				*/
			/*receive packets from whosSending if the protocol permits or if
			 * we are a destination then accept from everyone*/
					if(*(int *)id <= D-1)
					{
						int k;
				/*in this case we just receive every packet from the client*/
						for(k = 0; k < numPackets; k++)
						{
					/*receive k packets from the node and add their hops to
					 * total hops*/
							dataPacket recvPacket;

							if((numB = recv(recvSock, &recvPacket, 
							sizeof(dataPacket), 0)) == -1)
							{
								perror("recv");
								exit(1);
							}
					
							recvPacket.id = ntohs(recvPacket.id);

							recvPacket.tempStepsAfterGeneration = 
							ntohs(recvPacket.tempStepsAfterGeneration);

							totalHops += recvPacket.tempStepsAfterGeneration;

							totalPacketsReceived++;
						}
				
						if (send(sendSock, &numPackets, sizeof(int), 0) == -1)
			        		perror("send");
					}
					else
					{
				/*we can check in our buffer if we have more than P% space*/
						int myPackets, packetsAcceptable;

						myPackets = ListCount(myBuffer);

				/*we leave out P% space from our total buffer which has size 
				 * Q*/
						packetsAcceptable = (P*Q)/100;

						if(myPackets < (Q-packetsAcceptable))
						{
							int acceptable, k;
	
							dataPacket *recvPacket;

							dataPacket rPacket;

							acceptable = (Q-packetsAcceptable) - myPackets;

							if(acceptable > numPackets)
							{
								acceptable = numPackets;
							}

					/*send how many packets we can accept and receive them*/
							if (send(sendSock, &acceptable, sizeof(int), 0) 
							== -1)
		        				perror("send");

							for(k = 0; k < acceptable; k++)
							{
								recvPacket = malloc(sizeof(dataPacket) * 1);
			
								if((numB = recv(recvSock, &rPacket, 
								sizeof(dataPacket), 0)) == -1)
								{
									perror("recv");
									exit(1);
								}

								rPacket.id = ntohs(rPacket.id);

								rPacket.tempStepsAfterGeneration = 
								ntohs(rPacket.tempStepsAfterGeneration);
					
								recvPacket->id = rPacket.id;

								recvPacket->tempStepsAfterGeneration = 
								rPacket.tempStepsAfterGeneration;
							/*
							printf("received packet has tempStepsAfterGe: %d"
							"\n", rPacket.tempStepsAfterGeneration);*/
								totalHops += 
								recvPacket->tempStepsAfterGeneration;
								ListAppend(myBuffer, recvPacket);
/*
							printf("totalHops increased to: %d"
							"when the loopCounter was: %d\n", totalHops, 
							loopCounter);
*/
							}
					/*since we are here we received acceptable packets*/
							if (send(sendSock, &acceptable, sizeof(int), 0) 
							== -1)
		        				perror("send");
						}
						else
						{
					/*we can't accept any packets so just send 100 here*/
							int acceptable;

							acceptable = 0;

							if (send(sendSock, &acceptable, sizeof(int), 0) 
							== -1)
			        			perror("send");
						}
					}

			/*	}*/
				mutexsCheck[*(int *)id] = 1;
				sem_wait(&mutexs[*(int *)id]);
			}
		}

		if(*(int *)id == D && whosTransmiting == D-1)
		{
			loopCounter++;
			printf("1 incremented the loopCounter to %d\n", loopCounter);
			if(loopCounter > K-1)
			{
				flag = 0;
				continue;
			}
			whosTransmiting = N-1;
			if(!mutexsCheck[whosTransmiting])
			{
				printf("%d should be waiting but it is not\n", 
				whosTransmiting);
			}
			for(i = 0; i < N; i++)
			{
				if(i == D)
				{
					continue;
				}
			/*just an assertion*/	
				if(!mutexsCheck[i])
				{
				/*
				while(!mutexsCheck[i])
				{
					sleep(1);
				}
				*/
					printf("ERROR something wrong with the ordering for id: "
					"%d near the end\n", i);
				}
			}
			printf("All are waiting properly\n");

			mutexsCheck[whosTransmiting] = 0;

			sem_post(&mutexs[whosTransmiting]);

			mutexsCheck[*(int *)id] = 1;

			sem_wait(&mutexs[*(int *)id]);
		}
	}
	if(*(int *)id == D)
	{
		for(i = 0; i < N; i++)
		{
			if(i == D)
			{
				continue;
			}
			/*just an assertion*/	
			if(!mutexsCheck[i])
			{
				/*
				while(!mutexsCheck[i])
				{
					sleep(1);
				}
				*/
				printf("ERROR something wrong with the ordering for id: "
				"%d near the end\n", i);
			}
			mutexsCheck[i] = 0;
			sem_post(&mutexs[i]);
		}
		/*printf("%d was here\n", *(int *)id);*/
	}
	/*
	else
	{
		printf("%d reached here instead of %d\n", *(int *)id, D);
	}
	*/
	for(i = 0; i < N; i++)
	{
		if(i == *(int *)id)
		{
			continue;
		}
		printf("myId: %d, id: %d x = %d, y = %d\n", *(int *)id, i, map[i].x,
		map[i].y);
	}
	for(i = 0; i < N; i++)
	{
		if(i == *(int *)id)
		{
			continue;
		}
		close(receivingSockets[i]);
		close(sendingSockets[i]);
	}
	return NULL;
}








int main(int argc, char** argv)
{	
	pthread_t test;
	int *turn, i, turnTracker;

    if (argc != 7) 
	{
        fprintf(stderr,"usage: N: total nodes in the system, K: number of "
		"timesteps to run for, D: total number of destination nodes, R: "
		"possible transmission range for each node, B: total buffer space "
		"each node has (in number of packets), P: percentage of buffer "
		"space to leave out for local packets according to protocol\n");
        exit(1);
    }
	
	N = atoi(argv[1]);
	K = atoi(argv[2]);
	D = atoi(argv[3]);
	R = atoi(argv[4]);
	Q = atoi(argv[5]);
	P = atoi(argv[6]);

	waiting = -1;
	mutexs = malloc((sizeof(sem_t))* N);
	mutexsCheck = malloc(sizeof(int)*N);
	if(mutexsCheck == NULL)
	{
		printf("malloc eror\n");
	}
	turnTracker = 0;
	for(i = 0; i < N; i++)
	{
		turn = malloc(sizeof(int) * 1);
		*turn = turnTracker;
	
		sem_init(&mutexs[i], 0, 0);
		if ((pthread_create(&test, NULL, threadFunction2, turn)) != 0) {
    		perror("threadCreate");
		    exit(1);
  		}

		turnTracker++;
		
	}
	whosTurn = 0;
	accepting = 0;
	whosSending = 0;
	totalHops = 0;
	totalPacketsReceived = 0;
	srand(71);
	
	sem_post(&mutexs[0]);
	


	pthread_join(test, NULL);

	sleep(5);

	printf("totalPacketsReceived %d, totalHops %d\n", totalPacketsReceived,
	totalHops);
	return 0;
}
