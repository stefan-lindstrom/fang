#6068
DL Syllabus Garbler~
1 c 9
*~
  if (%actor.level% > 205 || (%cmd% != look & %cmd% != read) || (%actor.guild(22)% == Member) || (%actor.guild(22)% == GL))
     return 0
   else
     %send% %actor.name% The text is in some language you do not understand...
     return 1
   end
~
#6069
DL Syllabus Protection~
1 ghim 100
~
 if (%actor.level% > 205 || (%actor.guild(22)% == Member) || (%actor.guild(22)% == GL))
     return 1
   else
     %send% %actor.name% This book is not for you.
     return 0
   end
~
$~
