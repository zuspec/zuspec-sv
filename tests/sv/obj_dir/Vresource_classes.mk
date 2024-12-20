# Verilated -*- Makefile -*-
# DESCRIPTION: Verilator output: Make include file with class lists
#
# This file lists generated Verilated files, for including in higher level makefiles.
# See Vresource.mk for the caller.

### Switches...
# C11 constructs required?  0/1 (always on now)
VM_C11 = 1
# Timing enabled?  0/1
VM_TIMING = 0
# Coverage output mode?  0/1 (from --coverage)
VM_COVERAGE = 0
# Parallel builds?  0/1 (from --output-split)
VM_PARALLEL_BUILDS = 0
# Tracing output mode?  0/1 (from --trace/--trace-fst)
VM_TRACE = 0
# Tracing output mode in VCD format?  0/1 (from --trace)
VM_TRACE_VCD = 0
# Tracing output mode in FST format?  0/1 (from --trace-fst)
VM_TRACE_FST = 0

### Object file lists...
# Generated module classes, fast-path, compile with highest optimization
VM_CLASSES_FAST += \
	Vresource \
	Vresource___024root__DepSet_h6abd2bda__0 \
	Vresource___024root__DepSet_h81e32d44__0 \
	Vresource_resource_pkg__03a__03aA_data__Vclpkg__DepSet_h3deb6c1c__0 \
	Vresource_resource_pkg__03a__03aA_data__Vclpkg__DepSet_hb914e5fe__0 \
	Vresource_resource_pkg__03a__03aB_data__Vclpkg__DepSet_hbfeb7917__0 \
	Vresource_resource_pkg__03a__03aB_data__Vclpkg__DepSet_h3714db07__0 \
	Vresource_resource_pkg__03a__03aC_data__Vclpkg__DepSet_he26323cc__0 \
	Vresource_resource_pkg__03a__03aC_data__Vclpkg__DepSet_hfa8d352e__0 \
	Vresource_resource_pkg__03a__03aresource__Vclpkg__DepSet_he79401c8__0 \
	Vresource_resource_pkg__03a__03aresource__Vclpkg__DepSet_hfeba1332__0 \
	Vresource__main \

# Generated module classes, non-fast-path, compile with low/medium optimization
VM_CLASSES_SLOW += \
	Vresource___024root__Slow \
	Vresource___024root__DepSet_ha2097f72__0__Slow \
	Vresource___024root__DepSet_h81e32d44__0__Slow \
	Vresource_resource_pkg__Slow \
	Vresource_resource_pkg__DepSet_he044109f__0__Slow \
	Vresource_resource_pkg__03a__03aA_data__Vclpkg__Slow \
	Vresource_resource_pkg__03a__03aA_data__Vclpkg__DepSet_hb914e5fe__0__Slow \
	Vresource_resource_pkg__03a__03aB_data__Vclpkg__Slow \
	Vresource_resource_pkg__03a__03aB_data__Vclpkg__DepSet_h3714db07__0__Slow \
	Vresource_resource_pkg__03a__03aC_data__Vclpkg__Slow \
	Vresource_resource_pkg__03a__03aC_data__Vclpkg__DepSet_hfa8d352e__0__Slow \
	Vresource_resource_pkg__03a__03aresource__Vclpkg__Slow \
	Vresource_resource_pkg__03a__03aresource__Vclpkg__DepSet_hfeba1332__0__Slow \

# Generated support classes, fast-path, compile with highest optimization
VM_SUPPORT_FAST += \

# Generated support classes, non-fast-path, compile with low/medium optimization
VM_SUPPORT_SLOW += \
	Vresource__Syms \

# Global classes, need linked once per executable, fast-path, compile with highest optimization
VM_GLOBAL_FAST += \
	verilated \
	verilated_random \
	verilated_threads \

# Global classes, need linked once per executable, non-fast-path, compile with low/medium optimization
VM_GLOBAL_SLOW += \


# Verilated -*- Makefile -*-
