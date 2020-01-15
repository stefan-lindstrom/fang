#19100
OgierGuildEQ~
1 j 100
~
   if (%actor.level% < 200)
   if (%actor.guild(13)% == Member || %actor.guild(13)% == GL)
         return 1
      else
         %send% %actor.name% You are not a member of the Ogier!
         return 0
      end
   else
      return 1
   end
~
$~
