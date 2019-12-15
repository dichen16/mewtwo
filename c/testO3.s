	.file	"test.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%d "
	.text
	.p2align 4
	.type	main._omp_fn.0, @function
main._omp_fn.0:
.LFB19:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$136, %rsp
	.cfi_def_cfa_offset 160
	call	omp_get_thread_num
	leaq	8(%rsp), %rdx
	movl	$15, %ecx
	movslq	%eax, %rbx
	movq	%rdx, %rdi
	xorl	%eax, %eax
	rep stosq
	cmpl	$1, %ebx
	movl	$186, %edi
	sbbq	%rax, %rax
	andl	$30, %eax
	addq	$2, %rax
	movq	%rax, (%rsp)
	xorl	%eax, %eax
	call	syscall
	movq	%rsp, %rdx
	movl	$128, %esi
	movl	%eax, %edi
	call	sched_setaffinity
	leal	1(%rbx), %eax
	movl	%eax, %edx
	shrl	$31, %edx
	leal	(%rax,%rdx), %ebp
	andl	$1, %ebp
	subl	%edx, %ebp
	movslq	%ebp, %rbp
	jmp	.L4
	.p2align 4,,10
	.p2align 3
.L5:
	call	GOMP_barrier
.L4:
	call	GOMP_barrier
	movl	$.LC0, %edi
	xorl	%eax, %eax
	movl	$1, t(,%rbx,4)
	movl	t(,%rbp,4), %esi
	call	printf
	call	GOMP_barrier
	movl	$0, t(,%rbx,4)
	call	GOMP_single_start
	testb	%al, %al
	je	.L5
	movl	$10, %edi
	call	putchar
	jmp	.L5
	.cfi_endproc
.LFE19:
	.size	main._omp_fn.0, .-main._omp_fn.0
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB18:
	.cfi_startproc
	movl	$2, %edi
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	call	omp_set_num_threads
	xorl	%ecx, %ecx
	xorl	%edx, %edx
	xorl	%esi, %esi
	movl	$main._omp_fn.0, %edi
	call	GOMP_parallel
	.cfi_endproc
.LFE18:
	.size	main, .-main
	.comm	t,8,8
	.ident	"GCC: (GNU) 9.2.1 20190827 (Red Hat 9.2.1-1)"
	.section	.note.GNU-stack,"",@progbits
