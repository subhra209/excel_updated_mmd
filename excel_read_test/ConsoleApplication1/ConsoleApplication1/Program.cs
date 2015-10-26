
#define TCPIP

using OfficeOpenXml;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Data.OleDb;
using System.IO;
 using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Data.OleDb;



namespace ConsoleApplication1
{
   class Program
   {
           static AutoResetEvent waitEvent;

           static Socket clientConnection;
       static void Main(string[] args)
       {
#if (TCPIP)
            String ipAddress = ConfigurationSettings.AppSettings["IP"];
            int port = Convert.ToInt32(ConfigurationSettings.AppSettings["PORT"]);
            clientConnection = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            System.Net.IPAddress ipaddr = System.Net.IPAddress.Parse(ipAddress);
            clientConnection.Connect(ipAddress, port);
#endif
            waitEvent = new AutoResetEvent(false);
            int iRow = 1;

            String sourceFile = "C:\\Users\\jo\\Desktop\\test.xlsx";
            String tempFile = "C:\\Users\\jo\\Desktop\\test1.xlsx";
            String emptyCell = "      ";
            while (true)
            {
                String cellValue = String.Empty;

                if (File.Exists(tempFile))
                    System.IO.File.Delete(tempFile);

                System.IO.File.Copy(sourceFile, tempFile, true);

                using (ExcelPackage xlPackage = new ExcelPackage(new System.IO.FileInfo(tempFile)))
                {

                    

                    // get the first worksheet in the workbook
                    ExcelWorksheet worksheet = xlPackage.Workbook.Worksheets[1];
                   

                    // output the data in column 2

                    int iCol;
                    for (iCol = 1; iCol <= 6; iCol++)
                    {
                        string temp = worksheet.Cell(iRow, iCol).Value;

                        if ((temp.Length < 4) && (iCol > 2))
                        {
                            if( iCol == 3 )
                                while(temp.Length != 3 )
                                    temp += " ";

                            else
                                while (temp.Length != 4)
                                    temp += " ";
                        }
                       

                        cellValue += temp;

                        cellValue += " ";
                          
                   //     if (cellValue == "")
                   //         cellValue = emptyCell;

                    }

                    byte[] bytes = System.Text.Encoding.ASCII.GetBytes(cellValue);
                    List<Byte> byteList = new List<byte>();

                    byteList.Add(0x83);
                    byteList.AddRange(bytes);

                    Console.WriteLine("Cell({0},{1})={2}", iRow, iCol, cellValue);
#if (TCPIP)
                    clientConnection.Send(byteList.ToArray());
#endif
                    waitEvent.WaitOne(3000);
                    



                }



                //int check = System.IO.File.Exists("C:\Users\JO\Desktop\test.xlsx", @"C:\Users\JO\Desktop\test1.xlsx");
                
                // output the formula in row 6

             
                iRow++;

                if (iRow > 2)
                    iRow = 1;

            }
            
       }
   }
}
