#700
DaesDae'Mar GeQ~
1 j 100
~
   if (%actor.level% < 200)
   if (%actor.guild(14)% == Member || %actor.guild(14)% == GL)
         return 1
      else
         %send% %actor.name% You are not a member of Daes Dae'mar!
         return 0
      end
   else
      return 1
   end
~
#733
Cake loader...~
1 n 100
~
%load% obj 734
%load% obj 734
~
#739
Deas Cookie Trigger~
1 c 7
eat~
if (%arg% == "Cookie" || %arg% == "House")
  %send% %actor.name% !
else
  %send% %actor.name% %arg%
end
return 0
~
#848
A Question~
2 g 100
~
if (%actor.level% >199)
%send% %actor.name% &RWHAT DO YOU THINK YOU ARE DOING??&n
%echoaround% %actor.name% &W%actor.name% has entered the room.&n
~
$~
