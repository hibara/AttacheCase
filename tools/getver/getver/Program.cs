using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Diagnostics;

namespace ConsoleApplication1
{
	class Program
	{
		static void Main(string[] args)
		{
			if (args.Length < 1)
			{
				return;
			}
			string AppFilePath = Path.GetFullPath(args[0]);

			if (File.Exists(AppFilePath) == false)
			{
				return;
			}
			//ファイル情報を取得する
			FileVersionInfo vi = FileVersionInfo.GetVersionInfo(AppFilePath);
			//バージョン番号
			//Console.WriteLine("FileVersion:{0}", vi.FileVersion);
			//メジャー、マイナー、ビルド、プライベートパート番号
			Console.WriteLine("{0}{1}{2}{3}", vi.FileMajorPart, vi.FileMinorPart, vi.FileBuildPart, vi.FilePrivatePart);

#if DEBUG
			System.Console.ReadLine();
#endif

		}
	}
}
