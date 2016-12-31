
// 启动代码
.global _start

_start:
// 异常向量表
	b _reset							/* 复位时,cpu跳到0地址 */
	b halt								/* cpu遇到不能识别的指令时 */
	b halt								/* swi异常，进入svc模式 */
	b halt								/* 预取中止异常 */
	b halt								/* 数据访问异常 */
	b halt								/* 没用到 */
	ldr	pc, _irq						/* 中断异常 */
	b halt								/* 快中断异常 */
_reset:
	// 把外设的基地址告诉CPU
    ldr r0, =0x70000000 					//对于6410来说,内存(0x00000000～0x60000000),外设(0x70000000-0x7fffffff)
    orr r0, r0, #0x13						//外设大小:256M
    mcr p15,0,r0,c15,c2,4       			//把r0的值(包括了外设基地址+外设大小)写给cpu
	 

	// 关看门狗
	ldr r0, =0x7E004000
	mov r1, #0
	str r1, [r0] 

	// 设置栈
#define INSTALL_TO_NAND
#ifdef INSTALL_TO_NAND
	ldr sp, =8*1024
#else
	ldr sp, =0x0C002000
#endif
	
	// 开启icaches
#ifdef  CONFIG_SYS_ICACHE_OFF
	bic r0, r0, #0x00001000 				@ clear bit 12 (I) I-cache
#else
	orr r0, r0, #0x00001000 				@ set bit 12 (I) I-cache
#endif
	mcr p15, 0, r0, c1, c0, 0

	// 设置时钟
	bl clock_init
	
	// 初始化sdram
	bl sdram_init

	// 初始化NAND Flash
	bl nand_init

	// 初始化uart
	bl init_uart

	// 初始化lcd
	bl lcd_init
	// 初始化irq
	@bl irq_init

	// 重定位，把程序的代码段、数据段复制到它的链接地址去
	adr r0, _start   			
	ldr r1, =_start  			
	ldr r2, =bss_start      	
	sub r2, r2, r1
	cmp r0,r1
	beq clean_bss
	bl copy2ddr					
	cmp r0, #0
	bne halt

    b on_ddr
		
	// 清BSS，把BSS段对应的内存清零
clean_bss:
	ldr r0, =bss_start
	ldr r1, =bss_end
	mov r3, #0
clean_loop:
	cmp r0, r1
	beq on_ddr
	str r3, [r0], #4
	b clean_loop		
	// 跳转
on_ddr:

// 开中断
	mov r0, #0x53			
	msr CPSR_cxsf, r0

	ldr pc, =main

	// 中断异常
_irq:
	.word irq
irq:
	/* 保存现场 */
	ldr sp, =0x54000000				
	sub lr, lr, #4
	stmdb sp!, {r0-r12, lr}  
	/* 处理异常 */
	bl do_irq
	/* 恢复现场 */
	ldmia sp!, {r0-r12, pc}^  /* ^表示把spsr恢复到cpsr */


halt:
	b halt	
