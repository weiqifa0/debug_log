/*
** Copyright (c) 2023 The tiny51_os project. All rights reserved.
** Created by crisqifawei 2023
*/

#include "debug_function.h"

int main(){
  int a = 123;
  LOG(INFO, "Hello,World");
  LOG(INFO, "a = %d", a);
  return 0;
}