import pefile
import argparse
import binascii
import os

if __name__ in '__main__':
    try:
        parser = argparse.ArgumentParser( description = 'Extracts shellcode from a PE.' )
        parser.add_argument( '-f', help = 'Path to the source executable', type = str )
        parser.add_argument( '-o', help = 'Path to store the output raw binary', type = str )
        option = parser.parse_args()

        pe_src_file = option.f
        if pe_src_file is None or pe_src_file == '':
            pe_src_file = os.path.join(os.path.dirname(os.path.realpath(__file__)), '..', 'bin', 'shellcode.exe')

        output_file = option.o
        if output_file is None or output_file == '':
            output_file = os.path.join(os.path.dirname(os.path.realpath(__file__)), '..', 'bin', 'shellcode.bin')

        PeExe = pefile.PE(pe_src_file)
        PeSec = PeExe.sections[0].get_data()

        if PeSec.find( b'ENDOFCODE' ) != None:
            ScRaw = PeSec[ : PeSec.find( b'ENDOFCODE' ) ]
            print('----- Number of bytes: {}'.format( len( ScRaw ) ) )

            # print the hex string of the shellcode
            # usage in c/cpp will like: 
            # uint8_t shellcode[] = { 0x00, 0x00, 0x00, 0x00 };
            trimed_bytes = ScRaw.rstrip(b'\x00')
            hex_string = binascii.hexlify(trimed_bytes, ",", 1).decode()
            # add prefix `0x`
            hex_string = "0x" + hex_string 
            hex_string = hex_string.replace(",", ", 0x")
            # add ending `0x00`
            hex_string = hex_string + ", 0x00" 
            print('\n\n')
            print(hex_string)
            print('\n\n')

            f = open(output_file, 'wb+')
            f.write(ScRaw)
            f.close()
        else:
            print('[!] error: no ending tag')
    except Exception as e:
        print( '[!] error: {}'.format( e ) )

