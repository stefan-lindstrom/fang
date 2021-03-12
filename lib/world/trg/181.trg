#18100
&RDragonSworn Guild EQ&n~
1 j 100
~
   if (%actor.level% < 200)
   if (%actor.guild(10)% == Member || %actor.guild(10)% == GL)
         return 1
      else
         %send% %actor.name% You are not a member of the Dragonsworn!
         return 0
      end
   else
      return 1
   end
~
$~
