#25900
Entertainer GeQ~
1 j 100
~
   if (%actor.level% < 200)
   if (%actor.guild(15)% == Member || %actor.guild(15)% == GL)
         return 1
      else
         %send% %actor.name% You are not a member of the Entertainer Guild!
         return 0
      end
   else
      return 1
   end
~
$~
