########################## COMPARE PARSING #########################
#65 
/bin/echo test1		test2
#72
/bin/echo -n test1		test2

######################### PARSING HELL#############################
#13
export T=">>"
$T lol

#74
export T="<<"
echo segfault $T grep segfault

#75
export T='<<'
echo segfault $T grep segfault

#76
export T="|"
$T$T$T$T$T$T$T

#101
'''''''''''''''' echo ok

#124
cd --

######################### BUILTIN ######################################
#61
echo test1		test2

#68
echo -n test1		test2

#102
cd '/////' 2>/dev/null     

#105
cd "doesntexist" 2>/dev/null 

#107
cd "wtf" 2>/dev/null

#122
cd ../../

#131
pwd
cd ~
cd -
pwd

#132
pwd
cd -
pwd

#140
env what

#164
export ___TEST=123

#172
export _TEST=100

#186
export _TEST=123

#196
export ________=123

#200
export TEST+=100

#208 - 217
unset =

unset "="

unset ""=

unset =""

unset ==

unset ?

unset "?"

unset $

unset "$"

unset $""

# 225 - 230
unset TES.T

unset TES+T

unset TES=T

unset TES}T

unset TES{T

unset TES-T

# 232
unset _TEST

# 235 - 238
unset TE*ST

unset TES#T

unset TES@T

unset TES!T

# 240 -242
unset ============

unset +++++++

unset ________

# 249 - 252
unset TES^T

unset TES!T

unset TES\~T

/bin/echo $HOME

# 260
exit ""


################# SCMD ################################
#4
""
#7
touch "
"

################ CORRECTION ###############################
#2
""
#3
"   "
#4
"    "

#19
exit ""

################## PATH FAIL ####################################
#1
unset PATH
#5
cd -
#8
cd ~/Desktop/
#11
env -i ./minishell


################### GO WILD ####################################3
#10
export test="arg1	arg2"

#13 -15
echo "env | /usr/bin/wc -l" | env -i $MINISHELL_PATH"/"$EXECUTABLE
echo $?

echo "ls" | env -i $MINISHELL_PATH"/"$EXECUTABLE
echo $?

echo "unset PATH" | env -i $MINISHELL_PATH"/"$EXECUTABLE
echo $?





#compare parsing
65 ,72    
#parsing hell  
exit code error 1, 5-9  12
stderr  12-15 leak    74-76   101    
#buitlin
61 68 102 105 107 140 200 224 231 260   
#pipelin 
25 with leak-30  38-40 just leak 80-89 with leak   
#smcd 4
 7  
 #correction
 2 3 4 19   
 #path fail 
 8 11   
 #wild
 10 13-15