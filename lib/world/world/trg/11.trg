#1100
SeaFolk GeQ~
1 j 100
~
   if (%actor.level% < 200)
   if (%actor.guild(16)% == Member || %actor.guild(16)% == GL)
         return 1
      else
         %send% %actor.name% You are not a member of the Sea Folk!
         return 0
      end
   else
      return 1
   end
~
$~
