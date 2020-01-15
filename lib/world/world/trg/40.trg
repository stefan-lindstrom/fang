#4000
Rebel Geq~
1 j 100
~
   if (%actor.level% < 200)
   if (%actor.guild(5)% == Member || %actor.guild(5)% == GL)
         return 1
      else
         %send% %actor.name% You are not a member of the Rebel Tower!
         return 0
      end
   else
      return 1
   end
~
#4058
Anti Asha Trigger~
0 g 100
~
if (%direction% == gate && %actor.guild(8)% == Member)
  %teleport% %actor.name% 10186
end
~
#4076
SS Trig~
2 c 100
READ~
if (%actor.name% == Egwene)
  %send% %actor.name% \\c07"\\c11Come now Mother, You mustn't worry about that!\\c07" says the Keeper form your side as she takes the folder of papers from your hands before you can read them.\\c00
end
~
#4077
salidar board protector~
2 g 100
~
if (%actor.name% == Asin)
   %teleport% asin 10186
 end
~
#4078
Board Guard.~
2 c 100
*~
      if (%actor.level% < 200)
if (%actor.guild(5)% == Member || %actor.guild(5)% == GL || %actor.guild(2)% == Member || %actor.guild(2)% == GL )
            return 0
         else
            %send% %actor.name% You are not a member of this guild!
            %force% %actor.name% south
            return 0
         end
      else
         return 0
      end
~
$~
