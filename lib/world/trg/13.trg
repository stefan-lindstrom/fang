#1300
Two Rivers GEQ~
1 j 100
~
   if (%actor.level% < 200)
   if (%actor.guild(11)% == Member || %actor.guild(11)% == GL)
         return 1
      else
         %send% %actor.name% You are not a member of the Two Rivers!
         return 0
      end
   else
      return 1
   end
~
$~
