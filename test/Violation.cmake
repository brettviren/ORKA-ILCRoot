cmake_minimum_required(VERSION 2.8)

if(COMMAND CMAKE_POLICY)
  if(POLICY CMP0009)
    cmake_policy(SET CMP0009 NEW)
  endif(POLICY CMP0009)
endif(COMMAND CMAKE_POLICY)

#Run Checkilcroot to get latest violations
execute_process(COMMAND make -k check-all
  WORKING_DIRECTORY $ENV{ILC_INSTALL}
  OUTPUT_FILE CheckIlcroot.log )

find_program(svn_command svn)

#Function to count no of lines in a file equivalent to wc -l
function (wcl _nLinesVar fileName)
  file(READ ${fileName} fileData)
  if(fileData)
    if(WIN32)
      string(REGEX MATCHALL "\r\n" nLines ${fileData})
    else()
      string(REGEX MATCHALL "[\r\n]" nLines ${fileData})
    endif(WIN32)
    list(LENGTH nLines nLines)
    set(${_nLinesVar} ${nLines} PARENT_SCOPE)
  else()
    set(${_nLinesVar} -1 PARENT_SCOPE)
  endif()
endfunction()

file(GLOB_RECURSE violExc "$ENV{ILC_INSTALL}/*THerwig6.viol"                     
  "$ENV{ILC_INSTALL}/*IlcRawReaderDateV3.viol"            
  "$ENV{ILC_INSTALL}/*dateStream.viol"                      
  "$ENV{ILC_INSTALL}/TTherminator/*.viol"                         
  "$ENV{ILC_INSTALL}/*esdAna.viol"                         
  "$ENV{ILC_INSTALL}/*TIsajet*.viol"                              
  "$ENV{ILC_INSTALL}/*RecAna.viol"                         
  "$ENV{ILC_INSTALL}/*LHAPDFWrap.viol"                     
  "$ENV{ILC_INSTALL}/RAW/check/event.viol"                
  "$ENV{ILC_INSTALL}/THerwig/check/HCommon.viol"          
  "$ENV{ILC_INSTALL}/*HBTprocCOMMON.viol"                  
  "$ENV{ILC_INSTALL}/*EPOScommon.viol"                     
  "$ENV{ILC_INSTALL}/*DPMcommon.viol"                      
  "$ENV{ILC_INSTALL}/*Hcommon.viol"                        
  "$ENV{ILC_INSTALL}/*TPHICcommon.viol"                    
  "$ENV{ILC_INSTALL}/JETAN/check/fastjet/*.viol"                            
  "$ENV{ILC_INSTALL}/HLT/check/JET/fastjet/*.viol"                            
  "$ENV{ILC_INSTALL}/PYTHIA8/*.viol"                              
  "$ENV{ILC_INSTALL}/TEvtGen/*.viol"                              
  "$ENV{ILC_INSTALL}/LHAPDF/*.viol"                         
  "$ENV{ILC_INSTALL}/*HydCommon.viol"                      
  "$ENV{ILC_INSTALL}/*IlcHLTDataBuffer.viol")
file(GLOB_RECURSE violFiles "$ENV{ILC_INSTALL}/*.viol")  
list(REMOVE_ITEM violFiles ${violExc})

#Total list of files
list(LENGTH violFiles totf)
#Total no of lines
set(totl 0)
#Count the number Violations per file and sort 
foreach( violFile ${violFiles} )
  file(READ ${violFile} fileData)
  math(EXPR violLen "0")
  if(fileData)
    string(REGEX MATCHALL "violated:" violList ${fileData}) 
    list(LENGTH violList violLen)   
  endif(fileData)
  math(EXPR totl "${totl} + ${violLen}")
  math(EXPR violLen "999999 - ${violLen}") 
  list(APPEND violCount "${violLen}:${violFile}")
endforeach( violFile )  
list(SORT violCount)
list(LENGTH violCount max)
if(max GREATER 10)
  set(max 10)
endif(max GREATER 10)
message("==============================Overall Violations==============================\n")
message("COUNT : VIOLATION FILE NAME\n------------------------------------------------------------------------------")
#Print out top 10 violations 
foreach(i RANGE ${max})
  list(GET violCount ${i} line)
  string(LENGTH ${line} lineLen)
  math(EXPR lineLen "${lineLen}-7")
  string(SUBSTRING ${line} 0 6 count)
  string(SUBSTRING ${line} 7 ${lineLen} lineLen)

  string(REPLACE "check/" "" args ${lineLen})
  string(REPLACE "viol" "*" args ${args})
  string(REPLACE "$ENV{ILC_INSTALL}" "$ENV{ILC_ROOT}" args ${args})
  file(GLOB args "${args}")
  foreach(arg ${args})
   list(APPEND argList "${arg}")
  endforeach(arg)

  string(REPLACE "$ENV{ILC_INSTALL}/" "" module ${lineLen}) 
  math(EXPR count "999999 - ${count}")
  string(LENGTH ${count} digit)
  if(${digit} EQUAL 1)
    set(space "    ")
  elseif(${digit} EQUAL 2)
    set(space "   ")
  elseif(${digit} EQUAL 3)
    set(space "  ")
  endif(${digit} EQUAL 1)
  message("${space}${count} : ${module}")

endforeach(i)

set(tot 0)
set (anlys "${anlys}\n\n==============================Detailed Analysis==============================\n")
#Blaming people
set (anlys "${anlys}+--| IlcRoot\n")
foreach( blameFile ${argList} )
  string(REPLACE "$ENV{ILC_ROOT}/" "" mainFile ${blameFile})
  set(anlys "${anlys}   |\n")
  execute_process(COMMAND "${svn_command}" "blame" "${blameFile}"
    WORKING_DIRECTORY "$ENV{ILC_INSTALL}"
    OUTPUT_FILE tmpViols
    OUTPUT_VARIABLE tmpVio)
  wcl(fileLen "$ENV{ILC_INSTALL}/tmpViols")
  set(anlys "${anlys}   |--| ${mainFile} |-- ${fileLen} lines\n")  
  set(nameList "")
  file(STRINGS "$ENV{ILC_INSTALL}/tmpViols" violData)
  # file(READ "tmpViols" violData)
  # string(REGEX REPLACE "\n" ";" "violData" "${violData}")
  foreach(violLine ${violData})
    string(REGEX MATCH "[0-9]+[ ]+[a-zA-Z0-9]+" "name" "${violLine}")
    string(REGEX REPLACE "^[0-9]+[ ]+" "" "name" "${name}")
    list(APPEND nameList ${name})
    list(APPEND fullViolList ${name})
  endforeach(violLine)
  list(LENGTH violData tot)
  set (userList ${nameList})
  set(finale "")  
  if(userList)
    list(REMOVE_DUPLICATES userList)
  endif(userList)
  foreach(name ${userList})
    string(REGEX MATCHALL "${name}" "nameCount" "${nameList}")
    list(LENGTH nameCount nameCount)
    math(EXPR num "(${nameCount}*100)/${tot}")
    math(EXPR dec "((10000*${nameCount}/${tot}-100*${num}+5))")
    if( ${dec} GREATER 99 )
      set(dec 0)
      math(EXPR num "${num}+1")
    endif(${dec} GREATER 99)
    math(EXPR dec "${dec}/10")
    math(EXPR pcnt "99999 - ${num}${dec}")
    string(LENGTH ${num} digit)
    if(digit EQUAL 1)
      set(space "  ")
    elseif(digit EQUAL 2)
      set(space " ")
    endif(digit EQUAL 1) 
    list(APPEND finale "${pcnt}-${space}${num}.${dec}% | ${name} |-- ${nameCount} lines")
  endforeach(name)
  list(SORT finale)
  foreach(record ${finale})
    string(LENGTH ${record} recLen)
    math(EXPR recLen "${recLen}-6")
    string(SUBSTRING ${record} 6 ${recLen}  record)
    set(anlys "${anlys}      +--|${record}\n")
  endforeach(record)
endforeach( blameFile)

list(LENGTH fullViolList tot)
set(nameList ${fullViolList})
set(finale "")
#Sorting Names
if(nameList)
  list(REMOVE_DUPLICATES nameList)
endif(nameList)
foreach(name ${nameList})
  string(REGEX MATCHALL "${name}" "nameCount" "${fullViolList}")
  list(LENGTH nameCount nameCount)
  math(EXPR num "(${nameCount}*100)/${tot}")
  math(EXPR dec "((10000*${nameCount}/${tot}-100*${num}+5))")
  if( ${dec} GREATER 99 )
    set(dec 0)
    math(EXPR num "${num}+1")
  endif(${dec} GREATER 99)
  math(EXPR dec "${dec}/10")
  math(EXPR pcnt "99999 - ${num}${dec}")
  string(LENGTH ${num} digit)
  if(digit EQUAL 1)
    set(space "  ")
  elseif(digit EQUAL 2)
    set(space " ")
  endif(digit EQUAL 1) 
  list(APPEND finale "${pcnt}-${space}${num}.${dec}% | ${name}")
endforeach(name)

#Print Code Percentages
list(SORT finale)
message("\n\n==============================Overall Percentages==============================\n")
foreach(record ${finale})
  string(LENGTH ${record} recLen)
  math(EXPR recLen "${recLen}-6")
  string(SUBSTRING ${record} 6 ${recLen}  record)
  message("${record}")
endforeach(record)

#Print Detailed Analysis
message("${anlys}")
