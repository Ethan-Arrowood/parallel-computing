program imageTest
    use imgBinding
    use iso_c_binding, only : c_ptr,c_f_pointer,c_loc,c_null_char,c_char,c_int
    implicit none

    integer(kind=4), pointer, dimension(:,:) :: matrix
    integer(kind=4), pointer, dimension(:) :: dims
    type(c_ptr) :: c_matrix
    character(kind=c_char,len=100) :: filename,savename
    integer :: i,j

    allocate(dims(2))
    filename='image.jpg'//c_null_char

    c_matrix = imageToMat(filename, c_loc(dims))
    call c_f_pointer(c_matrix,matrix,(/dims(1),dims(2)/))
    write(*,*) dims(1),dims(2)

    !do work with matrix
    do i=1,dims(1)/2
        do j=1,dims(2)
            matrix(i,j)=matrix(i,j)+30
            if(matrix(i,j)>255) matrix(i,j)=255
        enddo
    enddo

    savename="newImage.jpg"//c_null_char
    call matToImage(savename,c_loc(matrix),c_loc(dims))


end program imageTest
