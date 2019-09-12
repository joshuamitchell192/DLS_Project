void EnableInterrupts(void);
void DisableInterrupts(void);

void EnableInterrupts(void){ 
	__asm ("    CPSIE  I\n" 
			"    BX     LR\n");
}


void DisableInterrupts(void){ 
	__asm ("    CPSID  I\n" 
			"    BX     LR\n");
}