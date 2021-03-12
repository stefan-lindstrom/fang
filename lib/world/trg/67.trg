#6700
&KDF GuildEq&n~
1 j 100
~
   if (%actor.level% < 200)
   if (%actor.guild(21)% == Member || %actor.guild(21)% == GL)
         return 1
      else
         %send% %actor.name% You are not a member of the Darkfriend Guild!
         return 0
      end
   else
      return 1
   end
~
#6701
&KChosen GeQ&n~
1 j 100
~
   if (%actor.level% < 200)
   if (%actor.guild(7)% == Member || %actor.guild(7)% == GL)
         return 1
      else
         %send% %actor.name% You are not a member of the Chosen Guild!
         return 0
      end
   else
      return 1
   end
~
$~
