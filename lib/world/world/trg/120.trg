#12000
&RBand of the Red Hand GEQ&n~
1 j 100
~
   if (%actor.level% < 200)
   if (%actor.guild(2)% == Member || %actor.guild(2)% == GL)
         return 1
      else
         %send% %actor.name% You are not a member of the Band of the Red Hand!
         return 0
      end
   else
      return 1
   end
~
#12001
Geq#2~
1 gij 100
~
   if (%actor.name% == Mat)
return 1
  else
  %send% %actor.name% &RYou are unable to use this item!&n
  return 0
end
~
#12002
Fox-Head Medallion Trigger~
1 c 9
*~
if (%actor.level% > 205 || (%cmd% != weave && %cmd% != cast && %cmd% != tieweave) || !%arg.contains(Mat)%)
  return 0
else
  %send% %actor.name% &WYour flows just seem to disappear as they touch Mat.&n
%send% mat &WYour foxhead medallion grows cold against your chest as %actor.name% attempts to weave on you!&n
  return 1
end
~
#12003
Cast~
1 c 7
cast~
%send% %actor.name% \\c08Your weave just seems to disappear.\\c00
%send% Mat \\c08Your fox-head medallion grows cold against your chest.\\c00
~
#12199
Mat's Medallion - Wear Trigger~
1 gj 100
~
if (%actor.level% < 200)
   if (%actor.name% == Mat)
      return 1
   else
      %send% %actor.name% You can't do that.
      return 0
   end
else
   return 1
end
~
$~
