#14400
Seanchan GuildEq~
1 j 100
~
   if (%actor.level% < 200)
   if (%actor.guild(12)% == Member || %actor.guild(12)% == GL)
         return 1
      else
         %send% %actor.name% You are not a member of the Seanchan!
         return 0
      end
   else
      return 1
   end
~
#14450
Seanchan Mannor Guard~
0 g 100
~
   if (%actor.level% < 200)
if (%direction% == South)
if (%actor.sex% == Male)
pemote %actor.name% nods to you as you leave.|nods to %actor.name% as he leaves.
else
pemote %actor.name% nods to you as you leave.|nods to %actor.name% as she leaves.
end
else
if (%actor.sex% == Male)
pemote %actor.name% eyes you as you approach.|eyes %actor.name% as he approaches.
else
pemote %actor.name% eyes you as you approach.|eyes %actor.name% as she approaches.
end
end
end
~
#14451
Seanchan Manor Guard - 2~
0 c 100
*~
if (%actor.level% < 200)
  if (%cmd% == S || %cmd% == South || %cmd% == So || %cmd% == Sou || %cmd% == Sout))
    if (%actor.guild(12)% == Member || %actor.guild(12)% == GL)
      return 0
    else
      pemote %actor.name% moves to block you from passing.|moves to block %actor.name% from passing.
    end
  else
    return 0
  end
end
~
$~
