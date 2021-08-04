#31501
Mob Reloader.~
0 f 100
~
load m %self.vnum%
~
#31562
Tancred's Spiffy Pass Trigger~
1 c 3
say~
 if (!%actor.isnpc%)
  if (%actor.level% < 200)
    if (%arg% == We love you Daene)
      %echoaround% %actor.name% %actor.name% decides to pray to %actor.hisher% favorite Imm, and disappears in a puff of brine scented smoke!
      %send% %actor.name% Really?  Thanks, you're so sweet!
      %teleport% %actor.name% 31562
    else
      return 0
    end
  end
end
~
#31597
Guard~
2 c 100
*~
  if (%actor.name% == Llewellyn || %actor.name% == Koltur || %actor.name% == Alyse || %actor.name% == Elyiandrys || %actor.name% == Anibelle || %actor.name% == Ciramina)
return 0
else
%send% %actor.name% You are not supposed to be here.
%damage% %actor.name% 25000
return 0
end
~
#31598
Exiter~
2 c 100
leave~
if (%actor.name% == Llewellyn)
 %send% %actor.name% Awww, you are leaving %actor.name% ?!?!?
 %send% %actor.name% -SNIFF- It's too bad you have to leave...!
 wait 5
 %send% %actor.name% You have left the Cabin.
 %teleport% %actor.name% 7760
elseif (%actor.name% == Alyse)
    %send% %actor.name% Awww, you are leaving %actor.name% ?!?!?
    %send% %actor.name% -SNIFF- It's too bad you have to leave...!
    wait 5
    %send% %actor.name% You have left the Cabin.
    %teleport% %actor.name% 7760
elseif (%actor.name% == Ciramina)
    %send% %actor.name% Awww, you are leaving %actor.name% ?!?!?
    %send% %actor.name% -SNIFF- It's too bad you have to leave...!
    wait 5
    %send% %actor.name% You have left the Cabin.
    %teleport% %actor.name% 7760
 elseif (%actor.name% == Anibelle)
     %send% %actor.name% Awww, you are leaving %actor.name% ?!?!?
     %send% %actor.name% -SNIFF- It's too bad you have to leave...!
     wait 5
     %send% %actor.name% You have left the Cabin.
     %teleport% %actor.name% 7760
else
 %send% %actor.name% You are trapped for all eternity.
 return 0
end
~
#31599
Enter-er~
2 c 100
enter~
if (%actor.name% == Llewellyn)
 %send% %actor.name% It is well that you have returned Llewellyn.
 wait 2
 %send% %actor.name% You have entered the Cabin.
 %teleport% %actor.name% 31599
elseif (%actor.name% == Alyse)
 %send% %actor.name% It is good for you to be here, beautiful.
 wait 2
 %send% %actor.name% You have entered the Cabin.
 %teleport% %actor.name% 31599
   elseif (%actor.name% == Anibelle)
    %send% %actor.name% It is good for you to be here, beautiful.
    wait 2
    %send% %actor.name% You have entered the Cabin.
    %teleport% %actor.name% 31599
  elseif (%actor.name% == Ciramina)
    %send% %actor.name% It is good for you to be here, beautiful.
    wait 2
    %send% %actor.name% You have entered the Cabin.
    %teleport% %actor.name% 31599
  elseif (%actor.name% == Koltur)
    %send% %actor.name% It is good for you to be here, beautiful.
    wait 2
    %send% %actor.name% You have entered the Cabin.
    %teleport% %actor.name% 31599
   else
    %send% %actor.name% There is nothing for you here.
    %damage% %actor.name% 10
    return 0
   end
~
$~
