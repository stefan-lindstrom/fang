#7100
AielGeq~
1 j 100
~
   if (%actor.level% < 200)
   if (%actor.guild(9)% == Member || %actor.guild(9)% == GL)
         return 1
      else
         %send% %actor.name% You are not a member of the Aiel!
         return 0
      end
   else
      return 1
   end
~
$~
