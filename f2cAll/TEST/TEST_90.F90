
    
subroutine bar(a,b,s)       
real, intent(inout) :: a(10), b(10)     
real, intent(inout) :: s  
      
integer i         
integer j(3)        

a = 0               ! assign-all         
j = (/ 5,2,8 /)     ! assign-list         
a = (/(i+1, i=1,10)/) ! implied-do         
a = cos(a) * 0.5    ! point-wise         
s = sum(a)          ! folding         
b(3:7) = a(1:5)     ! slicing         
b( : :2) = a( : :2) ! strides         
b(j) = a(j) + 100   ! indexing       
      
     
end  
