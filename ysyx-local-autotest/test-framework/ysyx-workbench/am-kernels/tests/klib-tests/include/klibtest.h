#ifndef __KLIBTEST_H__
#define __KLIBTEST_H__

#include <am.h>
#include <klib.h>
#include <klib-macros.h>

__attribute__((noinline))
void check(bool cond) {
  if (!cond) halt(1);
}

#endif
