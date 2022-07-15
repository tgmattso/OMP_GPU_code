#
#  USAGE:
#     make          ... to build the program
#     make test     ... to run the default test case
#
include make.def

#EXES= mm_testbed$(EXE) pi$(EXE) pi_recur$(EXE) pi_spmd$(EXE)  pi_loop$(EXE) pi_task$(EXE)
EXES= pi$(EXE) pi_recur$(EXE) pi_spmd$(EXE)  pi_loop$(EXE) pi_task$(EXE)

MM_OBJS  = mm_testbed.$(OBJ) mm_utils.$(OBJ) mm_tst_cases.$(OBJ) \
           mm_ijk.$(OBJ) mm_ikj.$(OBJ) mm_ikj_par.$(OBJ) \
	   mm_gpu.$(OBJ) mm_trans.$(OBJ) mm_gpu_block.$(OBJ) \
           mm_gpu_block_allocate_directive.$(OBJ) \
           mm_cublas.$(OBJ)

DIV_OBJS = div.$(OBJ) random.$(OBJ)

all: $(EXES)

pi$(EXE): pi.$(OBJ)  
	$(CLINKER) $(CFLAGS) -o pi$(EXE) pi.$(OBJ)   $(LIBS)

pi_recur$(EXE): pi_recur.$(OBJ)  
	$(CLINKER) $(CFLAGS) -o pi_recur$(EXE) pi_recur.$(OBJ)   $(LIBS)

pi_spmd$(EXE): pi_spmd.$(OBJ)  
	$(CLINKER) $(CFLAGS) -o pi_spmd$(EXE) pi_spmd.$(OBJ)   $(LIBS)

pi_loop$(EXE): pi_loop.$(OBJ)  
	$(CLINKER) $(CFLAGS) -o pi_loop$(EXE) pi_loop.$(OBJ)   $(LIBS)

pi_task$(EXE): pi_task.$(OBJ)  
	$(CLINKER) $(CFLAGS) -o pi_task$(EXE) pi_task.$(OBJ)   $(LIBS)

mm_testbed$(EXE): $(MM_OBJS) mm_utils.h
	$(CLINKER) $(CFLAGS) -o mm_testbed$(EXE) $(MM_OBJS) $(LIBS)

div$(EXE): $(DIV_OBJS) random.h
	$(CLINKER) $(CFLAGS) -o div$(EXE) $(DIV_OBJS)  $(LIBS)

test: $(EXES)
	$(PRE)mm_testbed$(EXE) 

clean:
	$(RM) $(EXES) *.$(OBJ)

.SUFFIXES:
.SUFFIXES: .c .cpp  .$(OBJ)

.c.$(OBJ):
	$(CC) $(CFLAGS) -c $<

.cpp.$(OBJ):
	$(CC) $(CFLAGS) -c $<
