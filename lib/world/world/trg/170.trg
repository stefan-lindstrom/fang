#17000
Whitecloak GEQ~
1 j 100
~
   if (%actor.level% < 200)
   if (%actor.guild(1)% == Member || %actor.guild(1)% == GL)
         return 1
      else
         %send% %actor.name% You are not a member of the Children of the Light!
         return 0
      end
   else
      return 1
   end
~
$~
