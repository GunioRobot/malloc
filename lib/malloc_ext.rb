# Namespace of the Hornetseye project.
module Hornetseye

  # Class for allocating raw memory and for doing pointer operations.
  class Malloc

    class << self

      # Create new Malloc object.
      #
      # @param [size] Number of bytes to allocate.
      # @return [Malloc] A new Malloc object.
      def new( size )
        retval = orig_new size
        retval.instance_eval { @size = size }
        retval
      end

      private :orig_new

    end

    # Number of bytes allocated.
    attr_reader :size

    # Operator for doing pointer arithmetic.
    #
    # @param [offset] Non-negative offset for pointer.
    # @return [Malloc] A new Malloc object.
    def +( offset )
      if offset > @size
        raise "Offset must not be more than #{@size} (but was #{offset})"
      end
      mark, size = self, @size - offset
      retval = orig_plus offset
      retval.instance_eval { @mark, @size = mark, size }
      retval
    end

    private :orig_plus

    # Read data from memory.
    #
    # @param [length] Number of bytes to read.
    # @return [String] A string containing the data.
    def read( length )
      raise "Only #{@size} bytes of memory left to read " +
        "(illegal attempt to read #{length} bytes)" if length > @size
      orig_read length
    end

    private :orig_read

    # Write data to memory.
    #
    # @param [string] A string with the data.
    # @return [String] Returns the parameter `string`.
    def write( string )
      if string.bytesize > @size
        raise "Must not write more than #{@size} bytes of memory " +
          "(illegal attempt to write #{string.bytesize} bytes)"
      end
      orig_write string
    end

    private :orig_write

  end

end

# The string class of the standard library.
class String

  unless method_defined? :bytesize

    # This method won't be overriden if it is defined already.
    #
    # Provided for compatibility with Ruby 1.8.6. Same as #size.
    def bytesize
      size
    end

  end
  
end