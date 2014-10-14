#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <cstdlib> 
#include <stdlib.h>
#include <ctime>

using namespace std;

void error_exit(){
   cout<<"Error occurred. Please enter valid timer value for complete display"<<endl;
   exit(1);
}

int main(int argc, char* argv[]){
int cou = 0;
	int c2p[2]; //child to parent pipe
	int p2c[2]; //parent to child pipe

	int i=0;
	pid_t chpid; // processor id variables

	string line, count;
	string ram[3000]; //memory of 1000 locations
	string systemStack[1000];//for system code

	char buf[10]; // buffer for reading pipes
	bool modeOfOperation=true;//to get the current mode of operation,true=user mode,false = system code mode

	//processor variables
	int pc=0, ir, sp=999,ac=0,x=0,y=0;
	int spInterrupt = 1999;
	
	//variable for command prompt
	char * nameTextFile;
	int timerValue;
	string tempTimerVal;
	
	//variable to store the data at the adress location given by dot
	string newAddressBuffer[3000];
	int tempAddrStr;
	int tempCoount=0;

	//code to get command promt values for file input and timer value
 	if(argv[1] != ""){
		nameTextFile=argv[1];
	}else{
		cout<<"Please enter the input file"<<endl;
	}
      if(argc > 2){
		timerValue=atoi(argv[2]);
	  }
	   

 	//file operation starts here
	ifstream myfile (nameTextFile);

	if ((pipe(p2c))== -1 || (pipe(c2p))== -1 )
	  exit(1);

	int nForkRet = fork();
	//case to check for read and write operation starts from here
	switch(nForkRet){
		case -1: //error occured in forking.
			exit(1);
					
		case 0: //child process
			if (myfile.is_open()){
				//to load the whole file into user memory
				while ( myfile.good() ){
					getline (myfile,line);
					//logic to detect the .address code
					int nDotPos = line.find(".");
					switch(nDotPos){
						case -1 :{//no dot is occured
							//code for normal file formatting and display the result
							LOOP1:if (line.length() == 0 ) continue;
							//cout<<line;
							int nSpacePos = line.find(" ");
							if(nSpacePos == 0) continue;
							line[nSpacePos] = '\0';	
								ram[i] = line;
								//cout<<"ram "<<ram[i]<<" " <<line<<endl;
								i++;
						break;
						}
						case 0: {//dot occurs
							 LOOP:line.replace(nDotPos, 1, "");
							 tempAddrStr = atoi(line.c_str());//got the address starting point
							 i=tempAddrStr;
							 if(myfile.good()){
								getline (myfile,line);
							 }
							 goto LOOP1;
 						break;
						}
					}
				}
				myfile.close();
			}else{
				cout << "Unable to open file";
			}
			//to find whether the operation to be performed is read or write
			while(read(p2c[0], buf, 10)){
				
				// temp1 will tell whether processor has demanded read or write instruction.
				char temp1 = buf[0];	

				if(temp1 == '1'){
				//this condition checks whether read or write. 1 when read
				
					char temp2[4]; // program counter will come in temp2
					memcpy( temp2, &buf[1], 4 );
					if(write(c2p[1], (void*)ram[atoi(temp2)].c_str(), 10)==-1)
					error_exit();
				}else{
					char temp2[4]; // program counter will come in temp2
					memcpy( temp2, &buf[1], 4 );
					int address = atoi(temp2);
					write(c2p[1], "1", 10);	//demo write to maintain sync
					read(p2c[0], buf, 10); //reads value in AC

					//stores value in AC at address specified.
					ram[address] = buf;
				}
			}
			
			_exit(0);
		default:
	while(1){
		//cout<<"value of pc is : "<<pc<<endl;
		//to get the value of ir, to select the instructions from the below mentioned cases
		stringstream ss;	
		ss.str("");
		ss << 1;
		ss << pc;
		count = ss.str();

		write(p2c[1], (void*)count.c_str(), 10);

		read(c2p[0], buf, 10);	
		
		if(timerValue != 0){
			ir=atoi(buf);
			timerValue--;
			if(timerValue == 0){
				pc = 1000;
			}
		}else{
			ir=atoi(buf);
		}

		switch(ir){

			case 1: //Load the value into the AC
					
					pc++;
					ss.str("");
					ss << 1;
					ss << pc;
					count = ss.str();
					write(p2c[1], (void*)count.c_str(), 10);
					read(c2p[0], buf, 10);
					ac = atoi(buf);
				break;
			
			case 2: // Load the value at the address into the AC
					
					pc++;
					ss.str("");
					ss << 1;
					ss << pc;
					count = ss.str();
					write(p2c[1], (void*)count.c_str(), 10);									
					read(c2p[0], buf, 10);
					ac = atoi(buf);
					ss.str("");
					ss << 1;
					ss << ac;
					count = ss.str();
					write(p2c[1], (void*)count.c_str(), 10);									
					read(c2p[0], buf, 10);
					ac = atoi(buf);
				break;
					
			case 3: // Load the value from the address found in the address into the AC
					
					pc++;
					ss.str("");
					ss << 1;
					ss << pc;
					count = ss.str();
					write(p2c[1], (void*)count.c_str(), 10);									
					read(c2p[0], buf, 10);
					ac = atoi(buf);
					ss.str("");
					ss << 1;
					ss << ac;
					count = ss.str();
					write(p2c[1], (void*)count.c_str(), 10);									
					read(c2p[0], buf, 10);
					ac = atoi(buf);
					ss.str("");
					ss << 1;
					ss << ac;
					count = ss.str();
					write(p2c[1], (void*)count.c_str(), 10);									
					read(c2p[0], buf, 10);
					ac = atoi(buf);
				break;
				
			case 4: // Load the value at (address+x) into the AC
					
					pc++;
					ss.str("");
					ss << 1;
					ss << pc;
					count = ss.str();
					write(p2c[1], (void*)count.c_str(), 10);									
					read(c2p[0], buf, 10);
					//get the overall address,which is( x + address)
					ac = atoi(buf)+x;
					ss.str("");
					ss << 1;
					ss << ac;
					count = ss.str();
					write(p2c[1], (void*)count.c_str(), 10);									
					read(c2p[0], buf, 10);
					ac = atoi(buf);
				break;
				
			case 5: // Load the value at (address+y) into the AC
					
					pc++;
					ss.str("");
					ss << 1;
					ss << pc;
					count = ss.str();
					write(p2c[1], (void*)count.c_str(), 10);									
					read(c2p[0], buf, 10);
					//get the overall address,which is( y + address)
					ac = atoi(buf)+y;
					ss.str("");
					ss << 1;
					ss << ac;
					count = ss.str();
					write(p2c[1], (void*)count.c_str(), 10);									
					read(c2p[0], buf, 10);
					ac = atoi(buf);
				break;
				
			case 6: // Load the value from  (sp+x) into the AC
					
				{
					ss.str("");
					ss << 1;
					int abc = sp+x+1;
					ss << abc;
					count = ss.str();
					write(p2c[1], (void*)count.c_str(), 10);									
					read(c2p[0], buf, 10);
					//get the overall address,which is( sp+x)
					ac = atoi(buf);
				break;
				}
					
			case 7:	//Store the value in the AC into the address
					
					pc++;
					ss.str("");
					//read from memory
					ss << 1;
					ss << pc;
					count = ss.str();
					write(p2c[1], (void*)count.c_str(), 10);
					read(c2p[0], buf, 10); // reads the address where AC needs to be copied
					ss.str("");
					//write to the memory
					ss << 2;
					ss << atoi(buf); // address at which value should be copied.									
					count = ss.str();
					write(p2c[1], (void*)count.c_str(), 10);
					read(c2p[0], buf, 10); // waiting for memory to copy address
					ss.str("");
					ss << ac;//value to be copied
					count = ss.str();
					write(p2c[1], (void*)count.c_str(), 10);
				break;
				
			case 8: //Geta a random number int from 1 to 100 into the AC
					//to get a random integer between 1 to 100 and store in ac
					srand((unsigned)time(0)); 
					//i want to generate the random number between 1-40 becaeuse for number greater than 40
					// it will give negative value, as the int limit is exceeded. 
					//If you want to check this, than change 40 to 100 in below mentioned code.
					ac = rand() % 40 + 1;
					cout<<"For "<<ac<<" numbers, the Fibonacci series is given below : "<<endl;
				break;

			case 9: // Put Port

					pc++;
					ss.str("");
					ss << 1;
					ss << pc;
					count = ss.str();
					write(p2c[1], (void*)count.c_str(), 10);								
					read(c2p[0], buf, 10); // read the port number
					if (buf[0] == '1'){
						cout<<ac;
					}else if(buf[0] == '2'){
						cout<<(char)ac;
					}else{
						cout<<ac<<endl;
					}
				break;
				
			case 10: //Add the value in X to the AC
					ac = ac + x;
				break;
			
			case 11: //Add the value in Y to the AC
					ac = ac + y;
				break;
			
			case 12: //Subtract the value in X from the AC
					ac = ac - x;
				break;

			case 13: //Subtract the value in Y from the AC
					ac = ac - y;
				break;

			case 14: // Copy the value of AC to x 
					x = ac;
				break;
			
			case 16: // Copy the value of AC to y
					y = ac;
				break;

			case 15: // Copy the value of x to AC
					ac = x;
				break;

			case 17: // Copy the value of y to AC
					ac = y;
				break;
				
			case 18: // Copy the value in AC to the SP
					sp = ac;
				break;
			
			case 19: // Copy the value in SP to the AC
					ac = sp;
				break;

			case 20: // Jump to address

					pc++; // to get the value of the jump address
					ss.str("");
					ss << 1;
					ss << pc;
					count = ss.str();
					write(p2c[1], (void*)count.c_str(), 10);								
					read(c2p[0], buf, 10); // gets the address
					pc = atoi(buf);
					pc--;
				break;

			case 21: // Jump to address if the value in AC is zero
					
					pc++; // to get the value of the jump address
					if (ac == 0){
						ss.str("");
						ss << 1;
						ss << pc;
						count = ss.str();
						write(p2c[1], (void*)count.c_str(), 10);								
						read(c2p[0], buf, 10); // gets the address
						pc = atoi(buf);
						pc--;
					}
				break;

			case 22: // Jump to address if the value in AC is not equal to zero
					
					pc++; // to get the value of the jump address
					if (ac != 0){
						ss.str("");
						ss << 1;
						ss << pc;
						count = ss.str();
						write(p2c[1], (void*)count.c_str(), 10);								
						read(c2p[0], buf, 10); // gets the address
						pc = atoi(buf);
					}
					pc--;
				break;

			case 23: // Push return address on to the stack and jump on the stack.
					
					//Putting AC on stack
				{
					pc++; // to get the value of the jump address
					ss.str("");
					ss << 1;
					ss << pc;
					count = ss.str();
					write(p2c[1], (void*)count.c_str(), 10);								
					read(c2p[0], buf, 10); // gets the address
					int tempPC = atoi(buf); //storing PC temporarily 
					pc++; // next address should be stored on the stack
					ss.str("");
					ss << 2;
					ss << sp; // address at which value should be copied.									
					count = ss.str();
					write(p2c[1], (void*)count.c_str(), 10);
					read(c2p[0], buf, 10); // waiting for memory to copy address
					ss.str("");
					ss << pc;//value to be copied
					count = ss.str();									
					write(p2c[1], (void*)count.c_str(), 10); //pushing the value to stack
					sp--;
					// now it will jump to the address
					pc = tempPC;
					pc--; //as PC will be incremented after this switch, it is decremented here.
				break;
				}

			case 24: // Pops return address from the stack and jumps to the address.

					//Getting the value of PC
					sp++; //making stack pointer to point PC
					ss.str("");
					ss << 1;
					ss << sp;
					count = ss.str();
					write(p2c[1], (void*)count.c_str(), 10);								
					read(c2p[0], buf, 10); // gets the PC
					pc = atoi(buf);
					pc--;
				break;

			case 25: // increment x
					x++;
				break;

			case 26: // decrement x
					x--;
				break;
				
			case 27: //Push AC on stack
				
					ss.str("");
					ss << 2;
					ss << sp; // address at which value should be copied.									
					count = ss.str();
					write(p2c[1], (void*)count.c_str(), 10);
					read(c2p[0], buf, 10); // waiting for memory to copy address
					ss.str("");
					ss << ac;//value to be copied
					count = ss.str();									
					write(p2c[1], (void*)count.c_str(), 10);
					sp--;
				break;
				
			case 28: // Pops from stack into AC

					// Getting value of AC
					sp++;
					ss.str("");
					ss << 1;
					ss << sp;
					count = ss.str();
					write(p2c[1], (void*)count.c_str(), 10);								
					read(c2p[0], buf, 10); // gets the AC
					ac = atoi(buf);
				break;
					
			case 29://set system mode,switch stack,push sp and pc,set new sp and pc
					//set the stack pointer to 1999, to store all registers at system stack
					//put current PC onto the stack
					ss.str("");
					ss << 2;
					ss << spInterrupt; // address at which value should be copied.									
					count = ss.str();
					write(p2c[1], (void*)count.c_str(), 10);
					read(c2p[0], buf, 10); // waiting for memory to copy address
					ss.str("");
					ss << pc;//value to be copied
					count = ss.str();									
					write(p2c[1], (void*)count.c_str(), 10);
					spInterrupt--; 
					//Putting current sp on stack
					ss.str("");
					ss << 2;
					ss << spInterrupt; // address at which value should be copied.									
					count = ss.str();
					write(p2c[1], (void*)count.c_str(), 10);
					read(c2p[0], buf, 10); // waiting for memory to copy address
					ss.str("");
					ss << sp;//value to be copied
					count = ss.str();									
					write(p2c[1], (void*)count.c_str(), 10);
					spInterrupt--; 
					//set new sp to spInterrupt and pc to 1500 address, becuase all further instruction are stored from 1500
					sp = spInterrupt;
					pc = 1500;
				break;
				
			case 30://restore register, set user mode
					// pop the value of pc and sp which were stored during interrupt 
					//Getting the value of pc
					sp++; //making stack pointer to point PC
					ss.str("");
					ss << 1;
					ss << sp;
					count = ss.str();
					write(p2c[1], (void*)count.c_str(), 10);								
					read(c2p[0], buf, 10); // gets the PC
					pc = atoi(buf);
					pc--;
					//Getting the value of sp
					sp++; //making stack pointer to point PC
					ss.str("");
					ss << 1;
					ss << sp;
					count = ss.str();
					write(p2c[1], (void*)count.c_str(), 10);								
					read(c2p[0], buf, 10); // gets the PC
					sp = atoi(buf);
					pc--;
				break;
				
			case 50:
					
					chpid = getpid();
					kill(chpid,0);
					_exit(1);
				break;

		}
		pc++;
	}
		waitpid(-1, NULL, 0);
	}
	return 0;
}
