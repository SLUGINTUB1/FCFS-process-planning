# Process Scheduler Simulation in Linux
## About project
This project implements a process scheduler in a Linux environment using POSIX standards. The application consists of three primary components: a dispatcher, generators, and executors.
The dispatcher manages the creation and termination of generator and executor processes, and configures system parameters such as the number of generators and executors, waiting time 
after a successful transfer and system state. Generators produce data for tabulation and send it to a shared message queue, while executors retrieve this data, perform the tabulation, 
and write the results to a text file. The synchronization between processes is managed using semaphores, with message queues facilitating data exchange. The project also includes context
switching via exec*() system calls and process creation using fork().

The system supports FСFS scheduling and uses a process buffer size of 8. The load and state of the system are visualized in both 2D and 3D graphs, with the QTableWidget component used for
displaying system status. Exclusive access to both the text file and buffer ensures data integrity. 


## Diagrams:
* [UseCase](https://drive.google.com/file/d/1oUt4hsyKNvFQSCg-npWgB7UBPJPjGZ_s/view?usp=drive_link)
* [State Chart](https://drive.google.com/file/d/1Z5aIpf14Mep4b6TNukfIwJOVUsND0EtZ/view?usp=drive_link)
* [Process Diagram](https://drive.google.com/file/d/13Q6xPf2bbMfaFSl-aaLMi-3l9FXJhiNQ/view?usp=drive_link)
 
## Screenshots
#### Main window of the dispatcher:
![зображення](https://github.com/user-attachments/assets/3b6de333-cb5f-45fa-b00f-92030d8f1ae8)
#### 2D graph showing CPU and RAM utilization:
![зображення](https://github.com/user-attachments/assets/d4447811-6ecf-4999-bc9e-3dea88927f30)

#### 3D graph showing RAM utilization:
![зображення](https://github.com/user-attachments/assets/a2a100e0-3404-49ab-bd60-6af8c50ac4ff)
