#300
HunterGeq~
1 j 100
~
   if (%actor.level% < 200)
   if (%actor.guild(17)% == Member || %actor.guild(17)% == GL)
         return 1
      else
         %send% %actor.name% You are not a member of the Hunters of the Horn!
         return 0
      end
   else
      return 1
   end
~
#317
a falling cow~
1 c 7
trap2~
      if (%cmd% == trap2)
   %send% %arg% &RA COW falls from the sky!!&n
   %echoaround% %arg% &rA COW falls from the sky, landing with a thud on %arg%!&n
   %damage% %arg% 25000
   return 1
   else
   return 0
   end
~
$~
