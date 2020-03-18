#include "shell.hh"
#include "unistd.h"
#include "fcntl.h"
#include "syscall.h"
#include <cerrno>
#include <string>

int main()
{ std::string input;

  // ToDo: Vervang onderstaande regel: Laad prompt uit bestand
  std::string prompt = "";

  while(true)
  { std::cout << prompt;                   // Print het prompt
    std::getline(std::cin, input);         // Lees een regel
    if (input == "new_file") new_file();   // Kies de functie
    else if (input == "ls") list();        //   op basis van
    else if (input == "src") src();        //   de invoer
    else if (input == "find") find();
    else if (input == "seek") seek();
    else if (input == "exit") return 0;
    else if (input == "quit") return 0;
    else if (input == "error") return 1;

    if (std::cin.eof()) return 0; } }      // EOF is een exit

void new_file()
{
  int text;
  int fd;
  char buff[10000];
  std::string fileName = "";
  std::cout << "Naam nieuw bestand: ";
  std::cin >> fileName;
  std::cout << "Tekst: ";
  text = read(0,buff,50);

  syscall(SYS_creat, fileName.c_str(), 0755);
  fd = syscall(SYS_open, fileName.c_str(), O_WRONLY, 0755);
  syscall(SYS_write,fd,buff,text);
  syscall(SYS_write,buff,text);
  syscall(SYS_close,fd);
}

void list() // ToDo: Implementeer volgens specificatie.
{
  int pid = syscall(SYS_fork);
  if(pid == 0){
    const char *pathname = "/bin/ls";
    const char *argv[] = {"-la",NULL};
    syscall(SYS_execve,pathname,argv,NULL);
  }
  else{
    syscall(SYS_wait4,pid,NULL,NULL);
  }
}

void find() // ToDo: Implementeer volgens specificatie.
{
  std::string search;
  std::cout << "geef een bestand om te zoeken: ";

  std::cin >> search;

  int p[2];
  syscall(SYS_pipe, p);

  if(syscall(SYS_fork) == 0)
  {
    syscall(SYS_close, p[0]);
    syscall(SYS_dup2, p[1], 1);

    const char* exec = "/bin/find";
    const char* args[] = {"/bin/find", ".", NULL};

    syscall(SYS_execve, exec, args, NULL);
    syscall(SYS_close, p[1]);
  }
  else
  {
    syscall(SYS_wait4, NULL, NULL, NULL);

    syscall(SYS_close, p[1]);
    syscall(SYS_dup2, p[0], 0);

    if(syscall(SYS_fork) == 0)
    {
      const char* exec = "/bin/grep";
      const char* args[] = {"/bin/grep", search.c_str(), NULL};

      syscall(SYS_execve, exec, args, NULL);
    }
    else
    {
      syscall(SYS_wait4, NULL, NULL, NULL);
      syscall(SYS_close, p[0]);
    }
  }
}

void seek() // ToDo: Implementeer volgens specificatie.
{
  syscall(SYS_creat,"seek",0755);
  int fdSeek = syscall(SYS_open,"seek",O_WRONLY);

  syscall(SYS_write,fdSeek,"x",1);
  syscall(SYS_lseek,fdSeek,5000000,SEEK_CUR);
  syscall(SYS_write,fdSeek,"x",1);
  syscall(SYS_close,fdSeek);

  syscall(SYS_creat,"loop",0755);
  int fdLoop = syscall(SYS_open,"loop",O_WRONLY);

  syscall(SYS_write,fdLoop,"x",1);
  for(unsigned i = 0; i < 5000000; i++){
    syscall(SYS_write,fdLoop,"",1);
  }
  syscall(SYS_write,fdLoop,"x",1);
  syscall(SYS_close,fdLoop);

}

void src() // Voorbeeld: Gebruikt SYS_open en SYS_read om de source van de shell (shell.cc) te printen.
{ int fd = syscall(SYS_open, "shell.cc", O_RDONLY, 0755); // Gebruik de SYS_open call om een bestand te openen.
  char byte[1];                                           // 0755 zorgt dat het bestand de juiste rechten krijgt (leesbaar is).
  while(syscall(SYS_read, fd, byte, 1))                   // Blijf SYS_read herhalen tot het bestand geheel gelezen is,
    std::cout << byte; }                                  //   zet de gelezen byte in "byte" zodat deze geschreven kan worden.
