module imgBinding
    use iso_c_binding

interface
    subroutine matToImage(string,mat_ptr,dims_ptr) bind(C,name='matToImage')
        use iso_c_binding
        character(kind=c_char) :: string(*)
        type(c_ptr),value :: mat_ptr
        type(c_ptr),value :: dims_ptr
    end subroutine matToImage

    type(C_PTR) function imageToMat(string,dims_ptr) bind(C,name='imageToMat')
        use iso_c_binding
        type(c_ptr),value :: dims_ptr
        character(kind=c_char) :: string(*)
    end function imageToMat
end interface
end module imgBinding
