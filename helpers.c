void EnableInterrupts(void);
void DisableInterrupts(void);

static inline void EnableInterrupts(void)
{ 
	 __asm("CPSIE i");
}


static inline void DisableInterrupts(void)
{ 
	__asm("CPSID i");
}
