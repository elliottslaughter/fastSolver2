ifndef LG_RT_DIR
$(error LG_RT_DIR variable is not defined, aborting build)
endif

#Flags for directing the runtime makefile what to include
DEBUG ?= 0                   # Include debugging symbols
OUTPUT_LEVEL ?= LEVEL_DEBUG  # Compile time print level
SHARED_LOWLEVEL ?= 0	  # Use the shared low level
USE_CUDA ?= 0
USE_GASNET ?= 1
USE_HDF ?= 0
#ALT_MAPPERS ?= 1		  # Compile the alternative mappers

# Put the binary file name here
OUTFILE	:= 	solver

# List all the application source files here
GEN_SRC	:= 	solver.cc \
		../src/hmatrix.cc ../src/tree.cc \
		../src/lmatrix.cc ../src/matrix.cc \
		../src/tasks/leaf_solve.cc ../src/tasks/node_solve.cc \
		../src/tasks/gemm_reduce.cc ../src/tasks/gemm_broadcast.cc \
		../src/tasks/gemm.cc ../src/tasks/gemm_inplace.cc \
		../src/tasks/node_solve_region.cc \
		../src/tasks/projector.cc ../src/tasks/reduce_add.cc \
		../src/tasks/init_matrix.cc ../src/tasks/clear_matrix.cc \
		../src/tasks/solver_tasks.cc ../src/tasks/display_matrix.cc \
		../src/tasks/dense_block.cc ../src/tasks/add_matrix.cc \
		../src/ptr_matrix.cc ../src/utility.cc \
		../src/tasks/scale_matrix.cc \
		../src/tasks/new_mapper.cc
#		../src/tasks/mapper.cc \
		../src/tasks/dist_mapper.cc ../src/tasks/spmd_mapper.cc

GEN_GPU_SRC	:=				# .cu files

# You can modify these variables, some will be appended to by the runtime makefile
INC_FLAGS	:= -I ../include/ -I ../include/tasks/
CC_FLAGS	:= \
		-DLEGION_IDS_ARE_64BIT \

NVCC_FLAGS	:=
GASNET_FLAGS	:=

ifneq (${CRAYPE_VERSION},)
# Cray systems include a special version of BLAS and LAPACK which is
# automatically linked into every application. Unfortunately, these
# versions include automatic multi-threading which breaks the
# application. To avoid this breakage, we use a custom version of BLAS.
LD_FLAGS 	:= ../netlib_blas/dgemm.o ../netlib_blas/dgesv.o ../netlib_blas/dgetrf.o ../netlib_blas/dgetrf2.o ../netlib_blas/dgetrs.o ../netlib_blas/dlamch.o ../netlib_blas/dlaswp.o ../netlib_blas/dscal.o ../netlib_blas/dtrsm.o ../netlib_blas/idamax.o ../netlib_blas/ieeeck.o ../netlib_blas/ilaenv.o ../netlib_blas/iparmq.o ../netlib_blas/lsame.o ../netlib_blas/xerbla.o
else
# otherwise use system BLAS/LAPACK
LD_FLAGS 	:= -llapack -lblas
endif

# enable legion spy
ifeq ($(strip $(DEBUG)),1)
CC_FLAGS += -DLEGION_SPY \
	-DPRIVILEGE_CHECKS \
	-DBOUNDS_CHECKS
endif

# dynamic linking on daint
ifeq ($(findstring daint,$(shell uname -n)),daint)
LD_FLAGS = -dynamic
endif

# mkl linking flags
#LD_FLAGS := -L/share/apps/intel/intel-14/mkl/lib/intel64/ \
	-L/share/apps/intel/intel-14/lib/intel64/ \
	-lmkl_intel_lp64 	\
	-lmkl_core		\
	-lmkl_sequential	\
	-lpthread 		\
	-lm

#	-lmkl_intel_thread 	\
	-liomp5 		\
	-lmkl_sequential	\



###########################################################################
#
#   Don't change anything below here
#   
###########################################################################

include $(LG_RT_DIR)/runtime.mk


#clean solver related
cleansolver:
	@$(RM) -rf $(OUTFILE) $(GEN_OBJS)  \
	*~  \
	../include/*~ ../src/*~ \
	../include/tasks/*~ ../src/tasks/*~

tar:	
	tar cvfz fastSolver.tgz Makefile \
	solver.cc \
	../src/hmatrix.cc ../src/tree.cc \
	../src/lmatrix.cc ../src/matrix.cc \
	../src/tasks/leaf_solve.cc ../src/tasks/node_solve.cc \
	../src/tasks/gemm_reduce.cc   ../src/tasks/gemm_broadcast.cc \
	../src/tasks/projector.cc ../src/tasks/reduce_add.cc \
	../src/tasks/init_matrix.cc ../src/tasks/clear_matrix.cc \
	../src/tasks/solver_tasks.cc ../src/tasks/display_matrix.cc \
	../src/tasks/dense_block.cc ../src/tasks/add_matrix.cc \
	../src/ptr_matrix.cc ../src/utility.cc \
	../src/tasks/scale_matrix.cc ../src/tasks/mapper.cc \
	../src/tasks/dist_mapper.cc \
	\
	../include/hmatrix.hpp ../include/tree.hpp \
	../include/lmatrix.hpp ../include/matrix.hpp \
	../include/tasks/leaf_solve.hpp ../include/tasks/node_solve.hpp \
	../include/tasks/gemm_reduce.hpp   ../include/tasks/gemm_broadcast.hpp \
	../include/tasks/projector.hpp ../include/tasks/reduce_add.hpp \
	../include/tasks/init_matrix.hpp ../include/tasks/clear_matrix.hpp \
	../include/tasks/solver_tasks.hpp ../include/tasks/display_matrix.hpp \
	../include/tasks/dense_block.hpp ../include/tasks/add_matrix.hpp \
	../include/ptr_matrix.hpp ../include/utility.hpp \
	../include/lapack_blas.hpp \
	../include/tasks/scale_matrix.hpp ../include/tasks/mapper.hpp \
	../include/tasks/dist_mapper.hpp

prof1:
	mpirun -H n0000 \
	-bind-to none -report-bindings \
	-x GASNET_IB_SPAWNER -x GASNET_BACKTRACE=1 \
	./solver \
	-machine 1 \
	-core 8 \
	-mtxlvl 6 \
	-ll:cpu 8 \
	-ll:gsize 0 \
	-hl:prof 1 \
	-level 4,legion_prof=2 \
	-logfile 1node_%.log 

#	gdb --args ./solver \

spy1:
	mpirun -H n0003 \
	-bind-to none -report-bindings \
	-x GASNET_IB_SPAWNER -x GASNET_BACKTRACE=1 \
	-x LEGION_FREEZE_ON_ERROR=1 \
	./solver \
	-machine 1 \
	-core 8 \
	-mtxlvl 6 \
	-ll:cpu 8 \
	-level 4,legion_spy=2 \
	-logfile 1node_%.log 

#	-x LEGION_FREEZE_ON_ERROR=1 \

spy2:
	mpirun -H n0002,n0003 \
	-bind-to none -report-bindings \
	-x GASNET_IB_SPAWNER -x GASNET_BACKTRACE=1 \
	-x LEGION_FREEZE_ON_ERROR=1 \
	./solver \
	-machine 2 \
	-core 8 \
	-mtxlvl 4 \
	-ll:cpu 8 \
	-level 4,legion_spy=2 \
	-logfile 2node_%.log &

#	-ll:gsize 0 \

prof2:
	mpirun -H n0000,n0001 \
	-bind-to none -report-bindings \
	-x GASNET_IB_SPAWNER -x GASNET_BACKTRACE=1 \
	./solver \
	-machine 2 \
	-core 8 \
	-mtxlvl 6 \
	-ll:cpu 8 \
	-hl:prof 2 \
	-level 4,legion_prof=2 \
	-logfile 2node_%.log

spy4:
	mpirun -H n0000,n0001,n0002,n0003 \
	-bind-to none -report-bindings \
	-x GASNET_IB_SPAWNER -x GASNET_BACKTRACE=1 \
	./solver \
	-machine 4 \
	-core 8 \
	-mtxlvl 6 \
	-ll:cpu 8 \
	-level 4,legion_spy=2 \
	-logfile 4node_%.log


prof4:
	mpirun -H n0000,n0001,n0002,n0003 \
	-bind-to none -report-bindings \
	-x GASNET_IB_SPAWNER -x GASNET_BACKTRACE=1 \
	./solver \
	-machine 4 \
	-core 8 \
	-mtxlvl 6 \
	-ll:cpu 8 \
	-hl:prof 4 \
	-level 4,legion_prof=2 \
	-logfile 4node_%.log


# --- legion profile ---

# node  : the number of processes, and there should be one
#  process for every node
# numa  : can be set to 'numactl -m 0 -N 0' to use numa node
# ncpu  :
# nutil :
# leaf  : legion leaf size
prof:
	mpirun -n $(node) -ppn 1  \
	-env MV2_SHOW_CPU_BINDING=1 \
	-env MV2_ENABLE_AFFINITY=0  \
	-env GASNET_IB_SPAWNER=mpi  \
	-env GASNET_BACKTRACE=1     \
	./solver		\
	-tasklvl 7		\
	-matrixlvl   7		\
	-ll:lmbsize 8192	\
	-ll:cpu    8		\
	-ll:csize  1000 	\
	-hl:disjointness	\
	-hl:prof   64		\
	-level 4,legion_prof=2 \
	-logfile node_%.log

#	-cat task,dma
#	-cat legion_prof	\
	-cat legion_spy	\
#	-ll:csize  1000 	\
#	-hl:sched  8192  	\
	-hl:window 8192		\
#	numactl			\
	-m 0 -N 0		\
	$(numa) 		\
	-ll:util   $(nutil)	\
	-ll:cpu    $(ncpu)	\
	-hl:prof   1		\

# --- Sapling commands ---

# check task execution (data movement)
spy:
	./test -ll:cpu 8 -ll:csize 3000 -level 2 -cat legion_spy
	python ~/legion/tools/legion_spy.py -p node_0.log

# --- legion profiler ---

# idx : node index
node:
	python ~/legion/tools/legion_prof.py -p node_$(idx).log
#	python ~/legion/tools/legion_prof.py -p node_$(idx).log \
	> legion_node.txt
