#include <stdio.h>
#include <iostream>

#if 0
void __attribute__((constructor)) _nss_http_init();
void __attribute__((destructor)) _nss_http_exit();

void _nss_http_init()
{
  std::cout << "nss-http: init" << std::endl;
}

void _nss_http_exit()
{
  std::cout << "nss-http: exit" << std::endl;
}
#endif