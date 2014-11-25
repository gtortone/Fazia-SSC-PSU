#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=cof
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/PowerSupply-SSC.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/PowerSupply-SSC.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=src/main.c src/setup.c src/cbuffer.c src/uartbuf.c src/frame.c src/isr.c src/functions.c src/spi-sw.c src/psu.c src/spi-dev.c src/comb-dev.c src/utils.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/setup.o ${OBJECTDIR}/src/cbuffer.o ${OBJECTDIR}/src/uartbuf.o ${OBJECTDIR}/src/frame.o ${OBJECTDIR}/src/isr.o ${OBJECTDIR}/src/functions.o ${OBJECTDIR}/src/spi-sw.o ${OBJECTDIR}/src/psu.o ${OBJECTDIR}/src/spi-dev.o ${OBJECTDIR}/src/comb-dev.o ${OBJECTDIR}/src/utils.o
POSSIBLE_DEPFILES=${OBJECTDIR}/src/main.o.d ${OBJECTDIR}/src/setup.o.d ${OBJECTDIR}/src/cbuffer.o.d ${OBJECTDIR}/src/uartbuf.o.d ${OBJECTDIR}/src/frame.o.d ${OBJECTDIR}/src/isr.o.d ${OBJECTDIR}/src/functions.o.d ${OBJECTDIR}/src/spi-sw.o.d ${OBJECTDIR}/src/psu.o.d ${OBJECTDIR}/src/spi-dev.o.d ${OBJECTDIR}/src/comb-dev.o.d ${OBJECTDIR}/src/utils.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/setup.o ${OBJECTDIR}/src/cbuffer.o ${OBJECTDIR}/src/uartbuf.o ${OBJECTDIR}/src/frame.o ${OBJECTDIR}/src/isr.o ${OBJECTDIR}/src/functions.o ${OBJECTDIR}/src/spi-sw.o ${OBJECTDIR}/src/psu.o ${OBJECTDIR}/src/spi-dev.o ${OBJECTDIR}/src/comb-dev.o ${OBJECTDIR}/src/utils.o

# Source Files
SOURCEFILES=src/main.c src/setup.c src/cbuffer.c src/uartbuf.c src/frame.c src/isr.c src/functions.c src/spi-sw.c src/psu.c src/spi-dev.c src/comb-dev.c src/utils.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/PowerSupply-SSC.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F26K20
MP_PROCESSOR_OPTION_LD=18f26k20
MP_LINKER_DEBUG_OPTION= -u_DEBUGCODESTART=0xfdc0 -u_DEBUGCODELEN=0x240 -u_DEBUGDATASTART=0xef4 -u_DEBUGDATALEN=0xb
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/src/main.o: src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	@${RM} ${OBJECTDIR}/src/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -p$(MP_PROCESSOR_OPTION) -I"./include" -ms -oa- -Ls -nw=2066 -nw=2060  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/src/main.o   src/main.c  -nw=2066 -nw=2060
	@${DEP_GEN} -d ${OBJECTDIR}/src/main.o 
	@${FIXDEPS} "${OBJECTDIR}/src/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/src/setup.o: src/setup.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/setup.o.d 
	@${RM} ${OBJECTDIR}/src/setup.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -p$(MP_PROCESSOR_OPTION) -I"./include" -ms -oa- -Ls -nw=2066 -nw=2060  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/src/setup.o   src/setup.c  -nw=2066 -nw=2060
	@${DEP_GEN} -d ${OBJECTDIR}/src/setup.o 
	@${FIXDEPS} "${OBJECTDIR}/src/setup.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/src/cbuffer.o: src/cbuffer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/cbuffer.o.d 
	@${RM} ${OBJECTDIR}/src/cbuffer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -p$(MP_PROCESSOR_OPTION) -I"./include" -ms -oa- -Ls -nw=2066 -nw=2060  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/src/cbuffer.o   src/cbuffer.c  -nw=2066 -nw=2060
	@${DEP_GEN} -d ${OBJECTDIR}/src/cbuffer.o 
	@${FIXDEPS} "${OBJECTDIR}/src/cbuffer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/src/uartbuf.o: src/uartbuf.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/uartbuf.o.d 
	@${RM} ${OBJECTDIR}/src/uartbuf.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -p$(MP_PROCESSOR_OPTION) -I"./include" -ms -oa- -Ls -nw=2066 -nw=2060  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/src/uartbuf.o   src/uartbuf.c  -nw=2066 -nw=2060
	@${DEP_GEN} -d ${OBJECTDIR}/src/uartbuf.o 
	@${FIXDEPS} "${OBJECTDIR}/src/uartbuf.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/src/frame.o: src/frame.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/frame.o.d 
	@${RM} ${OBJECTDIR}/src/frame.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -p$(MP_PROCESSOR_OPTION) -I"./include" -ms -oa- -Ls -nw=2066 -nw=2060  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/src/frame.o   src/frame.c  -nw=2066 -nw=2060
	@${DEP_GEN} -d ${OBJECTDIR}/src/frame.o 
	@${FIXDEPS} "${OBJECTDIR}/src/frame.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/src/isr.o: src/isr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/isr.o.d 
	@${RM} ${OBJECTDIR}/src/isr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -p$(MP_PROCESSOR_OPTION) -I"./include" -ms -oa- -Ls -nw=2066 -nw=2060  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/src/isr.o   src/isr.c  -nw=2066 -nw=2060
	@${DEP_GEN} -d ${OBJECTDIR}/src/isr.o 
	@${FIXDEPS} "${OBJECTDIR}/src/isr.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/src/functions.o: src/functions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/functions.o.d 
	@${RM} ${OBJECTDIR}/src/functions.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -p$(MP_PROCESSOR_OPTION) -I"./include" -ms -oa- -Ls -nw=2066 -nw=2060  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/src/functions.o   src/functions.c  -nw=2066 -nw=2060
	@${DEP_GEN} -d ${OBJECTDIR}/src/functions.o 
	@${FIXDEPS} "${OBJECTDIR}/src/functions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/src/spi-sw.o: src/spi-sw.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/spi-sw.o.d 
	@${RM} ${OBJECTDIR}/src/spi-sw.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -p$(MP_PROCESSOR_OPTION) -I"./include" -ms -oa- -Ls -nw=2066 -nw=2060  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/src/spi-sw.o   src/spi-sw.c  -nw=2066 -nw=2060
	@${DEP_GEN} -d ${OBJECTDIR}/src/spi-sw.o 
	@${FIXDEPS} "${OBJECTDIR}/src/spi-sw.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/src/psu.o: src/psu.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/psu.o.d 
	@${RM} ${OBJECTDIR}/src/psu.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -p$(MP_PROCESSOR_OPTION) -I"./include" -ms -oa- -Ls -nw=2066 -nw=2060  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/src/psu.o   src/psu.c  -nw=2066 -nw=2060
	@${DEP_GEN} -d ${OBJECTDIR}/src/psu.o 
	@${FIXDEPS} "${OBJECTDIR}/src/psu.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/src/spi-dev.o: src/spi-dev.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/spi-dev.o.d 
	@${RM} ${OBJECTDIR}/src/spi-dev.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -p$(MP_PROCESSOR_OPTION) -I"./include" -ms -oa- -Ls -nw=2066 -nw=2060  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/src/spi-dev.o   src/spi-dev.c  -nw=2066 -nw=2060
	@${DEP_GEN} -d ${OBJECTDIR}/src/spi-dev.o 
	@${FIXDEPS} "${OBJECTDIR}/src/spi-dev.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/src/comb-dev.o: src/comb-dev.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/comb-dev.o.d 
	@${RM} ${OBJECTDIR}/src/comb-dev.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -p$(MP_PROCESSOR_OPTION) -I"./include" -ms -oa- -Ls -nw=2066 -nw=2060  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/src/comb-dev.o   src/comb-dev.c  -nw=2066 -nw=2060
	@${DEP_GEN} -d ${OBJECTDIR}/src/comb-dev.o 
	@${FIXDEPS} "${OBJECTDIR}/src/comb-dev.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/src/utils.o: src/utils.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/utils.o.d 
	@${RM} ${OBJECTDIR}/src/utils.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -p$(MP_PROCESSOR_OPTION) -I"./include" -ms -oa- -Ls -nw=2066 -nw=2060  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/src/utils.o   src/utils.c  -nw=2066 -nw=2060
	@${DEP_GEN} -d ${OBJECTDIR}/src/utils.o 
	@${FIXDEPS} "${OBJECTDIR}/src/utils.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
else
${OBJECTDIR}/src/main.o: src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	@${RM} ${OBJECTDIR}/src/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"./include" -ms -oa- -Ls -nw=2066 -nw=2060  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/src/main.o   src/main.c  -nw=2066 -nw=2060
	@${DEP_GEN} -d ${OBJECTDIR}/src/main.o 
	@${FIXDEPS} "${OBJECTDIR}/src/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/src/setup.o: src/setup.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/setup.o.d 
	@${RM} ${OBJECTDIR}/src/setup.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"./include" -ms -oa- -Ls -nw=2066 -nw=2060  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/src/setup.o   src/setup.c  -nw=2066 -nw=2060
	@${DEP_GEN} -d ${OBJECTDIR}/src/setup.o 
	@${FIXDEPS} "${OBJECTDIR}/src/setup.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/src/cbuffer.o: src/cbuffer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/cbuffer.o.d 
	@${RM} ${OBJECTDIR}/src/cbuffer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"./include" -ms -oa- -Ls -nw=2066 -nw=2060  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/src/cbuffer.o   src/cbuffer.c  -nw=2066 -nw=2060
	@${DEP_GEN} -d ${OBJECTDIR}/src/cbuffer.o 
	@${FIXDEPS} "${OBJECTDIR}/src/cbuffer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/src/uartbuf.o: src/uartbuf.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/uartbuf.o.d 
	@${RM} ${OBJECTDIR}/src/uartbuf.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"./include" -ms -oa- -Ls -nw=2066 -nw=2060  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/src/uartbuf.o   src/uartbuf.c  -nw=2066 -nw=2060
	@${DEP_GEN} -d ${OBJECTDIR}/src/uartbuf.o 
	@${FIXDEPS} "${OBJECTDIR}/src/uartbuf.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/src/frame.o: src/frame.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/frame.o.d 
	@${RM} ${OBJECTDIR}/src/frame.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"./include" -ms -oa- -Ls -nw=2066 -nw=2060  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/src/frame.o   src/frame.c  -nw=2066 -nw=2060
	@${DEP_GEN} -d ${OBJECTDIR}/src/frame.o 
	@${FIXDEPS} "${OBJECTDIR}/src/frame.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/src/isr.o: src/isr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/isr.o.d 
	@${RM} ${OBJECTDIR}/src/isr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"./include" -ms -oa- -Ls -nw=2066 -nw=2060  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/src/isr.o   src/isr.c  -nw=2066 -nw=2060
	@${DEP_GEN} -d ${OBJECTDIR}/src/isr.o 
	@${FIXDEPS} "${OBJECTDIR}/src/isr.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/src/functions.o: src/functions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/functions.o.d 
	@${RM} ${OBJECTDIR}/src/functions.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"./include" -ms -oa- -Ls -nw=2066 -nw=2060  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/src/functions.o   src/functions.c  -nw=2066 -nw=2060
	@${DEP_GEN} -d ${OBJECTDIR}/src/functions.o 
	@${FIXDEPS} "${OBJECTDIR}/src/functions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/src/spi-sw.o: src/spi-sw.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/spi-sw.o.d 
	@${RM} ${OBJECTDIR}/src/spi-sw.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"./include" -ms -oa- -Ls -nw=2066 -nw=2060  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/src/spi-sw.o   src/spi-sw.c  -nw=2066 -nw=2060
	@${DEP_GEN} -d ${OBJECTDIR}/src/spi-sw.o 
	@${FIXDEPS} "${OBJECTDIR}/src/spi-sw.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/src/psu.o: src/psu.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/psu.o.d 
	@${RM} ${OBJECTDIR}/src/psu.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"./include" -ms -oa- -Ls -nw=2066 -nw=2060  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/src/psu.o   src/psu.c  -nw=2066 -nw=2060
	@${DEP_GEN} -d ${OBJECTDIR}/src/psu.o 
	@${FIXDEPS} "${OBJECTDIR}/src/psu.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/src/spi-dev.o: src/spi-dev.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/spi-dev.o.d 
	@${RM} ${OBJECTDIR}/src/spi-dev.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"./include" -ms -oa- -Ls -nw=2066 -nw=2060  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/src/spi-dev.o   src/spi-dev.c  -nw=2066 -nw=2060
	@${DEP_GEN} -d ${OBJECTDIR}/src/spi-dev.o 
	@${FIXDEPS} "${OBJECTDIR}/src/spi-dev.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/src/comb-dev.o: src/comb-dev.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/comb-dev.o.d 
	@${RM} ${OBJECTDIR}/src/comb-dev.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"./include" -ms -oa- -Ls -nw=2066 -nw=2060  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/src/comb-dev.o   src/comb-dev.c  -nw=2066 -nw=2060
	@${DEP_GEN} -d ${OBJECTDIR}/src/comb-dev.o 
	@${FIXDEPS} "${OBJECTDIR}/src/comb-dev.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/src/utils.o: src/utils.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/utils.o.d 
	@${RM} ${OBJECTDIR}/src/utils.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"./include" -ms -oa- -Ls -nw=2066 -nw=2060  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/src/utils.o   src/utils.c  -nw=2066 -nw=2060
	@${DEP_GEN} -d ${OBJECTDIR}/src/utils.o 
	@${FIXDEPS} "${OBJECTDIR}/src/utils.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/PowerSupply-SSC.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE)   -p$(MP_PROCESSOR_OPTION_LD)  -w -x -u_DEBUG -m"PSU.map" -l"/home/tortone/Desktop/PowerSupply-SSC"  -z__MPLAB_BUILD=1  -u_CRUNTIME -z__MPLAB_DEBUG=1 -z__MPLAB_DEBUGGER_PICKIT2=1 $(MP_LINKER_DEBUG_OPTION) -l ${MP_CC_DIR}/../lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/PowerSupply-SSC.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
else
dist/${CND_CONF}/${IMAGE_TYPE}/PowerSupply-SSC.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE)   -p$(MP_PROCESSOR_OPTION_LD)  -w  -m"PSU.map" -l"/home/tortone/Desktop/PowerSupply-SSC"  -z__MPLAB_BUILD=1  -u_CRUNTIME -l ${MP_CC_DIR}/../lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/PowerSupply-SSC.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
