#2602
Guard~
2 g 100
~
   if (%actor.level% < 200)
   if (%actor.guild(19)% == Member || %actor.guild(19)% == GL)
         return 1
      else
         %send% %actor.name% &WYou are not a member of the KIN!&n
         return 0
      end
   else
      return 1
   end
   
~
$~
