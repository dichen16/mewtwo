#!/usr/bin/python
from bcc import BPF  

program = """
    int hello(void *ctx){
        bpf_trace_printk("Hello BPF!\\n");
        return 0;
    }
"""

b = BPF(text=program)

# clone syscall is when you create a new process
b.attach_kprobe(event="__x64_sys_clone", fn_name="hello")
# write to /sys/kernel/debug/tracing/trace_pipe
b.trace_print()

