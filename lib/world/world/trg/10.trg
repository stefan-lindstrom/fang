#1000
White Tower GEQ~
1 j 100
~
   if (%actor.level% < 200)
   if (%actor.guild(3)% == Member || %actor.guild(3)% == GL)
         return 1
      else
         %send% %actor.name% You are not a member of the White Tower!
         return 0
      end
   else
      return 1
   end
~
$~
