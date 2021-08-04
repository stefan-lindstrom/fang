#521
WK Board Room~
2 g 100
~
 
       if (%actor.level% < 200)
         if (%actor.guild(20)% == Member || %actor.guild(20)% == GL)
               return 1
            else
               %send% %actor.name% &m&uYou are not a allowed here.&n
               return 0
            end
         else
            return 1
         end
~
$~
