#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;

Context* __am_irq_handle(Context *c) {
  if (user_handler) {
    Event ev = {0};
    switch (c->mcause) {
      case EVENT_YIELD: ev.event = EVENT_YIELD; break;
      default: ev.event = EVENT_ERROR; break;
    }

    c = user_handler(ev, c);
    assert(c != NULL);
  }

  return c;
}

extern void __am_asm_trap(void);

bool cte_init(Context*(*handler)(Event, Context*)) {
  // initialize exception entry
  asm volatile("csrw mtvec, %0" : : "r"(__am_asm_trap));

  // register event handler
  user_handler = handler;
  return true;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
    // 创建上下文结构
    uint32_t aligned_stack_addr = (uint32_t)kstack.end & ~(sizeof(uint32_t) - 1);
    Context *context = (Context *)aligned_stack_addr - sizeof(Context);//从末尾创建上下文结构，因为栈空间足够，因此不用担心不够
    context->mepc = (unsigned int)entry;
    //传递参数args给entry
    context->gpr[10] = (unsigned int)arg; //gpr[10] 即a0

    printf("stack start = 0x%x,end = 0x%x\n",kstack.start,kstack.end);
    printf("create context = 0x%x,size = 0x%x, end = 0x%x\n",context,sizeof(Context),context + sizeof(Context));
    return context;
}

void yield() {
#ifdef __riscv_e
  asm volatile("li a5, -1; ecall");
#else
  asm volatile("li a7, -1; ecall");
#endif
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}
