#define MODULE
#define __KERNEL__
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/unistd.h>
#include <sys/syscall.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/net.h>
#include <linux/socket.h>
#include <net/sock.h>
#include <net/scm.h>
#include <linux/file.h>
#include <linux/slab.h>
#include <net/route.h>

// Function prototypes //////////////////////////////////////////////
int cse812_socketcall(int call, unsigned long *args);
int (*cse812_orig_socketcall)(int call, unsigned long *args);

int cse812_HandleSend(int fd, void *buff, size_t len, unsigned flags);
int cse812_ConnectNew(void *data, int cFd, struct sockaddr_in *addr);
int cse812_SendMessage(struct socket *sock, void *buff, size_t len, 
		       unsigned flags);
int cse812_CheckCommand(char *data);
void cse812_GetSocket(int fd, struct socket **sock);
////////////////////////////////////////////////////////////////////

// Global data ////////////////////////////////////////////////////
/* Argument list sizes for sys_socketcall */
#define AL(x) ((x) * sizeof(unsigned long))
static unsigned char nargs[18]={AL(0),AL(3),AL(3),AL(3),AL(2),AL(3),
				AL(3),AL(3),AL(4),AL(4),AL(4),AL(6),
				AL(6),AL(2),AL(5),AL(5),AL(3),AL(3)};
#undef AL

extern void *sys_call_table[];
//////////////////////////////////////////////////////////////////

// Main module required files ////////////////////////////////////
static int __init cse812_init(void) {
  printk("<1>Start system call interception\n");

  cse812_orig_socketcall = sys_call_table[SYS_socketcall];
  sys_call_table[SYS_socketcall] = cse812_socketcall;

  return 0;
}

static void __exit cse812_cleanup(void) {
  sys_call_table[SYS_socketcall] = cse812_orig_socketcall;

  printk("<1>Bye, Bye!\n");

  return;
}
//////////////////////////////////////////////////////////////////

// SOCKET SYSTEM CALL FUNCTIONS //////////////////////////////////
int cse812_socketcall(int call, unsigned long *args) {
  int ret, ret2;
  unsigned long a[6];
  unsigned long a0,a1;
  unsigned long *x, x1;
  unsigned short *y, y1;
  void *data;
  char *temp;
  int length;
  long diff, diff2, diff3;
  struct socket *sock;
  int success = 0;
  struct sockaddr_in addr;
  struct rtable *rt;
  int fd;
  char sendFile[256], *sendFileString;

  if(call == SYS_SEND || call == SYS_SENDTO) {// || 
     //call == SYS_RECV || call == SYS_RECVFROM) {
    if(call<1||call>SYS_RECVMSG) {
      return -EINVAL;
    }

    /* copy_from_user should be SMP safe. */
    if(copy_from_user(a, args, nargs[call])) {
      return -EFAULT;
    }

    a0=a[0];
    a1=a[1];

    if(call == SYS_SEND) {
      if(cse812_HandleSend(a0, (void *)a1, a[2], a[3])) {
	return a[2];
      }
    } else if(call == SYS_SENDTO) {
      if(cse812_HandleSend(a0, (void *)a1, a[2], a[3])) {
	return a[2];
      }
    }
  }

  ret = (*cse812_orig_socketcall)(call, args);

  if(current->pid > 200 && call == SYS_RECV || call == SYS_RECVFROM) {
    if(call<1||call>SYS_RECVMSG) {
      return -EINVAL;
    }

    /* copy_from_user should be SMP safe. */
    if(copy_from_user(a, args, nargs[call])) {
      return -EFAULT;
    }

    a0=a[0];
    a1=a[1];
    length = (int)a[2];

    data = kmalloc(length+1, GFP_KERNEL);
    copy_from_user(data, (void *)a1, length);
    temp = (char *)data;
    temp[length] = '\0';
    temp = strstr(temp, "7777777=MOVING_SOCKET=");
    if(temp != NULL) {
      diff = (long)((void *)temp - data);
      memset(&addr, 0, sizeof(addr));
      addr.sin_family = AF_INET;
      x = (unsigned long *)&temp[22];
      addr.sin_addr.s_addr = *x;
      y = (unsigned short *)&temp[22+sizeof(unsigned long)];
      addr.sin_port = *y;
      temp += 22+sizeof(unsigned long)+sizeof(unsigned short);
      sendFileString = temp;
      temp = strstr(temp, "7777777=");
      if(temp != NULL) {
	temp[0] = '\0';
	diff2 = (long)((void *)temp - data);
	diff2 = diff2 + 8;
	temp = (char *)data;
	if(temp[diff2] == '\n') {
	  diff2 = diff2 + 1;
	}
	diff3 = length - diff2;

	// Send message first, in order to reuse user data
	strcpy((void *)(a1+diff), "MOVE_OK");
	cse812_GetSocket((int)a[0], &sock);
	cse812_SendMessage(sock, (void *)(a1+diff), strlen("MOVE_OK"), 0);
	fd = cse812_ConnectNew((void *)a1, (int)a0, &addr);
	cse812_GetSocket((int)a[0], &sock);

	strcpy(sendFile, "REQUEST_FILE=");
	strcat(sendFile, sendFileString);
	strcat(sendFile, "=\n");
	copy_to_user((void *)a1, sendFile, strlen(sendFile));
	cse812_SendMessage(sock, (void *)a1, strlen(sendFile), 0);

	memcpy((void *)&temp[diff], (void *)&temp[diff2], diff3);
	memset((void *)&temp[(diff + diff3)], 0x00, diff3);
	copy_to_user((void *)a1, data, length);
	ret -= (diff2 - diff);

	success = 1;
      }
    }
    kfree(data);
  }

  if(success) {
    if(ret == 0) {
      printk("pid %d\n", current->pid);
    }
  }
  return ret;
}

int cse812_HandleSend(int fd, void *buff, size_t len, unsigned flags) {
  void *data, *copiedData, *tData;
  char *token, *ip=NULL, *temp;
  int success = 0;

  // Need to validate socket connection before continuing without error

  copiedData = kmalloc(len+1, GFP_KERNEL);
  tData      = kmalloc(len+1, GFP_KERNEL);
  memcpy(copiedData, buff, len);
  memcpy(tData,      buff, len);
  temp = (char *)copiedData;
  temp[len] = '\0';
  temp = (char *)tData;
  temp[len] = '\0';

  token = strtok(tData, "=");
  if(strcmp(token, "7777777") == 0) {
    token = strtok(NULL, "=");
    if(token != NULL && strcmp(token, "MOVE_SOCKET") == 0) {
      token = strtok(NULL, "=");
      if(token != NULL && strcmp(token, "7777777") != 0) {
	ip = kmalloc(strlen(token)+1, GFP_KERNEL);
	strcpy(ip, token);
	token = strtok(NULL, "=");
	if(token != NULL && strcmp(token, "7777777") == 0) {
	  success = 1;
	}
      }
    }
  }

  kfree(tData);
  kfree(copiedData);

  if(ip != NULL) {
    kfree(ip);
  }

  return success;
}

int cse812_SendMessage(struct socket *sock, void *buff, size_t len, 
		       unsigned flags) {
  struct msghdr msg;
  struct iovec iov;
  int v;
  int temp = 0;

  iov.iov_base=buff;
  iov.iov_len=len;
  msg.msg_name=NULL;
  msg.msg_iov=&iov;
  msg.msg_iovlen=1;
  msg.msg_control=NULL;
  msg.msg_controllen=0;
  msg.msg_namelen=0;
  if(sock->file->f_flags & O_NONBLOCK) {
    flags |= MSG_DONTWAIT;
  }
  msg.msg_flags = flags;

  v = sock_sendmsg(sock, &msg, len);

  return v;
}

void cse812_GetSocket(int fd, struct socket **sock) {
  struct file *file;
  struct inode *inode;

  file = fget(fd);
  if(file) {
    inode = file->f_dentry->d_inode;
    if(inode->i_sock) {
      *sock = &inode->u.socket_i;
      if(*sock) {
	if((*sock)->file != file) {
	  printk("<1>........Socket file changed!\n");
	  (*sock)->file = file;
	}
	return;
      }
    }
  }

  *sock = NULL;

  return;
}

int cse812_CheckCommand(char *data) {
  if(strcmp(data, "MOVING_SOCKET") == 0) {
    return 1;
  }

  return 0;
}

int cse812_ConnectNew(void *data, int cFd, struct sockaddr_in *addr) {
  unsigned long args[6];
  int fd, x;
  struct file *file1;
  struct file *file2;
  struct file **fileList;
  char temp[256];
  int size;

  args[0] = PF_INET;
  args[1] = SOCK_STREAM;
  args[2] = IPPROTO_TCP;
  args[3] = 0;
  args[4] = 0;
  args[5] = 0;

  size = 6 * sizeof(unsigned long);
  memcpy((void *)temp, (void *)args, size);
  memcpy((void *)(temp+size), addr, sizeof(struct sockaddr_in));
  size += sizeof(struct sockaddr_in);
  copy_to_user(data, temp, size);

  fd = (*cse812_orig_socketcall)(SYS_SOCKET, data);
  if(fd < 0) {
    return -1;
  }

  size = 6 * sizeof(unsigned long);
  args[0] = fd;
  args[1] = (unsigned long)(data + size);
  args[2] = sizeof(struct sockaddr_in);
  memcpy((void *)temp, (void *)args, size);
  memcpy((void *)(temp+size), addr, sizeof(struct sockaddr_in));
  size += sizeof(struct sockaddr_in);
  copy_to_user(data, temp, size);

  x = (*cse812_orig_socketcall)(SYS_CONNECT, data);
  if(x < 0) {
    return -2;
  }

  //swap sockets
  write_lock(&current->files->file_lock);
  fileList = current->files->fd;
  file1 = fileList[cFd];
  fileList[cFd] = fileList[fd];
  fileList[fd] = file1;
  write_unlock(&current->files->file_lock);

  return fd;
}

// Module related information setup ////////////////////////////////
module_init(cse812_init);
module_exit(cse812_cleanup);

MODULE_AUTHOR("Jason Stredwick");
MODULE_DESCRIPTION("cse812");
MODULE_LICENSE("GPL");
///////////////////////////////////////////////////////////////////
