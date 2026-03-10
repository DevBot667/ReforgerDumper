[WorkbenchPluginAttribute(name: "Script Dump Manager Plugin", category: "Script Dumper", shortcut: "Ctrl+D", wbModules: {"ResourceManager"}, awesomeFontCode: 0xf0c5)]
class ReforgerDumperPluginSettings: ResourceManagerPlugin
{
	// --- File type toggles (Plugins -> Settings) ---
	[Attribute("1", UIWidgets.CheckBox, "Include .acp files", category: "File types to dump")]
	bool m_acp;
	
	[Attribute("1", UIWidgets.CheckBox, "Include .ae files", category: "File types to dump")]
	bool m_ae;
	
	[Attribute("1", UIWidgets.CheckBox, "Include .agf files", category: "File types to dump")]
	bool m_agf;
	
	[Attribute("1", UIWidgets.CheckBox, "Include .agr files", category: "File types to dump")]
	bool m_agr;
	
	[Attribute("1", UIWidgets.CheckBox, "Include .anm files", category: "File types to dump")]
	bool m_anm;
	
	[Attribute("1", UIWidgets.CheckBox, "Include .asi files", category: "File types to dump")]
	bool m_asi;
	
	[Attribute("1", UIWidgets.CheckBox, "Include .ast files", category: "File types to dump")]
	bool m_ast;
	
	[Attribute("1", UIWidgets.CheckBox, "Include .asy files", category: "File types to dump")]
	bool m_asy;
	
	[Attribute("1", UIWidgets.CheckBox, "Include .aw files", category: "File types to dump")]
	bool m_aw;
	
	[Attribute("1", UIWidgets.CheckBox, "Include .bt files", category: "File types to dump")]
	bool m_bt;
	
	[Attribute("1", UIWidgets.CheckBox, "Include .c files", category: "File types to dump")]
	bool m_c;
	
	[Attribute("1", UIWidgets.CheckBox, "Include .conf files", category: "File types to dump")]
	bool m_conf;
	
	[Attribute("1", UIWidgets.CheckBox, "Include .ct files", category: "File types to dump")]
	bool m_ct;
	
	[Attribute("1", UIWidgets.CheckBox, "Include .emat files", category: "File types to dump")]
	bool m_emat;
	
	[Attribute("1", UIWidgets.CheckBox, "Include .et files", category: "File types to dump")]
	bool m_et;
	
	[Attribute("1", UIWidgets.CheckBox, "Include .fnt files", category: "File types to dump")]
	bool m_fnt;
	
	[Attribute("1", UIWidgets.CheckBox, "Include .gamemat files", category: "File types to dump")]
	bool m_gamemat;
	
	[Attribute("1", UIWidgets.CheckBox, "Include .json files", category: "File types to dump")]
	bool m_json;
	
	[Attribute("1", UIWidgets.CheckBox, "Include .layer files", category: "File types to dump")]
	bool m_layer;
	
	[Attribute("1", UIWidgets.CheckBox, "Include .layout files", category: "File types to dump")]
	bool m_layout;
	
	[Attribute("1", UIWidgets.CheckBox, "Include .pap files", category: "File types to dump")]
	bool m_pap;
	
	[Attribute("1", UIWidgets.CheckBox, "Include .physmat files", category: "File types to dump")]
	bool m_physmat;
	
	[Attribute("1", UIWidgets.CheckBox, "Include .ptc files", category: "File types to dump")]
	bool m_ptc;
	
	[Attribute("1", UIWidgets.CheckBox, "Include .sig files", category: "File types to dump")]
	bool m_sig;
	
	[Attribute("1", UIWidgets.CheckBox, "Include .siga files", category: "File types to dump")]
	bool m_siga;
	
	[Attribute("1", UIWidgets.CheckBox, "Include .styles files", category: "File types to dump")]
	bool m_styles;
	
	[Attribute("1", UIWidgets.CheckBox, "Include .terr files", category: "File types to dump")]
	bool m_terr;
	
	[Attribute("1", UIWidgets.CheckBox, "Include .vhcsurf files", category: "File types to dump")]
	bool m_vhcsurf;
	
	[Attribute("1", UIWidgets.CheckBox, "Include .ent files", category: "File types to dump")]
	bool m_ent;
	
	[Attribute("1", UIWidgets.CheckBox, "Include .edds files", category: "File types to dump")]
	bool m_edds;
	
	[Attribute("1", UIWidgets.CheckBox, "Include .xob files", category: "File types to dump")]
	bool m_xob;
	
	[Attribute("1", UIWidgets.CheckBox, "Include .imageset files", category: "File types to dump")]
	bool m_imageset;
	
	[Attribute("1", UIWidgets.CheckBox, "Include .wav files", category: "File types to dump")]
	bool m_wav;
	
	// --- Settings ---
	[Attribute("1", UIWidgets.CheckBox, "Ignore files in the WorkbenchGame directory", category: "Settings")]
	bool m_IgnoreWorkbenchGame;
	
	[Attribute("0", UIWidgets.CheckBox, "Skip files that already exist in the Dump folder", category: "Settings")]
	bool m_SkipExisting;
	
	[Attribute("1", UIWidgets.CheckBox, "Fix GUID references in text files after dump (rewrites ResourceName paths)", category: "GUID Fix")]
	bool m_FixGUIDs;
	
	[Attribute("1", UIWidgets.CheckBox, "Generate .meta files for each dumped resource", category: "GUID Fix")]
	bool m_GenerateMeta;
	
	[Attribute("1", UIWidgets.CheckBox, "Export guid_map.csv mapping file for debugging", category: "GUID Fix")]
	bool m_ExportGuidMap;
	
	[Attribute("", UIWidgets.EditBox, "Target addon prefix for rewritten paths (e.g. MyMod). Leave empty to use relative paths without addon prefix.", category: "GUID Fix")]
	string m_TargetAddonName;
	
	// Counters for progress tracking
	protected int m_iDumpedCount;
	protected int m_iSkippedCount;
	protected int m_iErrorCount;
	protected int m_iRewrittenCount;
	
	// GUID mapping: ResourceName (with GUID) -> output relative path
	// Key = original ResourceName string (e.g. "{ABCD1234}path/to/file.et")
	// Value = new local relative path (e.g. "core/system/materials/file.emat")
	protected ref map<string, string> m_GuidMap;
	
	// List of dumped text files that may contain ResourceName references
	protected ref array<string> m_TextFilesToRewrite;
	
	// Text-based file extensions that can contain ResourceName/GUID references
	protected ref array<string> m_TextExtensions;
	
	[ButtonAttribute("Dump")]
	void DumpFiles()
	{
		array<string> extensions = {};
		GetEnabledFileExtensions(extensions);
		
		if (extensions.IsEmpty())
		{
			Print("[ScriptDumper] No file extensions selected. Aborting.");
			return;
		}
		
		m_iDumpedCount = 0;
		m_iSkippedCount = 0;
		m_iErrorCount = 0;
		m_iRewrittenCount = 0;
		m_GuidMap = new map<string, string>();
		m_TextFilesToRewrite = {};
		
		// Extensions whose file content is text-based and may contain ResourceName references
		m_TextExtensions = {
			"et", "emat", "layout", "conf", "layer", "ct", "bt",
			"gamemat", "physmat", "ptc", "styles", "fnt", "imageset",
			"acp", "agf", "agr", "anm", "asi", "ast", "asy",
			"sig", "siga", "terr", "vhcsurf", "ent", "json", "pap", "ae", "aw"
		};
		
		Print("[ScriptDumper] === Phase 1/3: Dumping files ===");
		Print("[ScriptDumper] Starting dump for " + extensions.Count() + " extension type(s)...");
		Workbench.SearchResources(SearchResourcesCallback, extensions);
		Print("[ScriptDumper] Dump complete. Dumped: " + m_iDumpedCount + " | Skipped: " + m_iSkippedCount + " | Errors: " + m_iErrorCount);
		Print("[ScriptDumper] Collected " + m_GuidMap.Count() + " GUID mappings.");
		
		// Phase 2: Export GUID map
		if (m_ExportGuidMap && m_GuidMap.Count() > 0)
		{
			Print("[ScriptDumper] === Phase 2/3: Exporting GUID map ===");
			ExportGuidMap();
		}
		
		// Phase 3: Rewrite GUID references in text files
		if (m_FixGUIDs && m_TextFilesToRewrite.Count() > 0 && m_GuidMap.Count() > 0)
		{
			Print("[ScriptDumper] === Phase 3/3: Rewriting GUID references in " + m_TextFilesToRewrite.Count() + " text files ===");
			RewriteGuidReferences();
			Print("[ScriptDumper] Rewritten references in " + m_iRewrittenCount + " files.");
		}
		
		Print("[ScriptDumper] === All done! ===");
		Print("[ScriptDumper] Dumped: " + m_iDumpedCount + " | Skipped: " + m_iSkippedCount + " | Errors: " + m_iErrorCount + " | Rewritten: " + m_iRewrittenCount);
		Print("[ScriptDumper] Move files out of the profile directory for better Workbench performance, then restart.");
	}
	
	void SearchResourcesCallback(ResourceName resName, string filePath = "")
	{
		if (filePath == string.Empty)
		{
			PrintFormat("[ScriptDumper] Empty filepath for %1", resName);
			m_iErrorCount++;
			return;
		}

		int colonIndex = filePath.IndexOf(":");
		if (colonIndex == -1)
		{
			PrintFormat("[ScriptDumper] Invalid path format (no colon): %1", filePath);
			m_iErrorCount++;
			return;
		}
		
		if (m_IgnoreWorkbenchGame && filePath.Contains("WorkbenchGame"))
		{
			m_iSkippedCount++;
			return;
		}

		// "$core:system/materials/file.emat" -> rootDir = "core"
		string rootDir = filePath.Substring(1, colonIndex - 1);
		
		// Skip profile directory to avoid writing into our own dump output
		if (rootDir.Contains("profile") || rootDir.Contains("Profile"))
		{
			m_iSkippedCount++;
			return;
		}
		
		// Build output path: "Dump/core/system/materials/file.emat"
		string relativePath = filePath.Substring(colonIndex + 1, filePath.Length() - colonIndex - 1);
		string localPath = "Dump/" + rootDir + "/" + relativePath;
		string fullOutputPath = "$profile:" + localPath;
		
		// Skip if file already exists and user opted to skip
		if (m_SkipExisting && FileIO.FileExist(fullOutputPath))
		{
			m_iSkippedCount++;
			return;
		}
		
		// Create directory tree
		EnsureDirectoryExists(localPath);
		
		// Copy the file using filePath as source (binary-safe)
		WriteFile(filePath, resName, fullOutputPath);
		
		// --- GUID mapping ---
		// Store the mapping: original ResourceName -> new relative path
		// ResourceName in Enfusion looks like "{GUID}addon:path/to/file.ext"
		// We need to map from the original ResourceName to the new path
		string resNameStr = resName.GetPath();
		string newRelativePath = rootDir + "/" + relativePath;
		
		if (resNameStr != string.Empty)
		{
			// Store full ResourceName string as key (includes GUID)
			m_GuidMap.Set(resNameStr, newRelativePath);
			
			// Also store just the path portion (without $addon: prefix) for matching
			string pathOnly = resNameStr;
			int resColonIdx = resNameStr.IndexOf(":");
			if (resColonIdx != -1)
			{
				pathOnly = resNameStr.Substring(resColonIdx + 1, resNameStr.Length() - resColonIdx - 1);
			}
			// Also remove GUID prefix if present: "{GUID}path" -> "path"
			int braceClose = pathOnly.IndexOf("}");
			if (braceClose != -1)
			{
				pathOnly = pathOnly.Substring(braceClose + 1, pathOnly.Length() - braceClose - 1);
			}
			if (pathOnly != string.Empty)
				m_GuidMap.Set(pathOnly, newRelativePath);
		}
		
		// Also store filePath as a key
		m_GuidMap.Set(filePath, newRelativePath);
		
		// Store path without the $addon: prefix as well
		string filePathWithoutPrefix = filePath.Substring(colonIndex + 1, filePath.Length() - colonIndex - 1);
		if (filePathWithoutPrefix != string.Empty)
			m_GuidMap.Set(filePathWithoutPrefix, newRelativePath);
		
		// Track text files for GUID rewrite phase
		if (IsTextFile(fullOutputPath))
		{
			m_TextFilesToRewrite.Insert(fullOutputPath);
		}
		
		// Generate .meta file if requested
		if (m_GenerateMeta)
		{
			GenerateMetaFile(resName, fullOutputPath, newRelativePath);
		}
	}
	
	// Check if a file is a text-based resource file
	protected bool IsTextFile(string path)
	{
		string lowerPath = path;
		// No .ToLower() in Enfusion, so check both cases via extension matching
		for (int i = 0; i < m_TextExtensions.Count(); i++)
		{
			if (path.EndsWith("." + m_TextExtensions[i]))
				return true;
		}
		return false;
	}
	
	// Generate a .meta file for the dumped resource
	protected void GenerateMetaFile(ResourceName resName, string outputPath, string newRelativePath)
	{
		string metaPath = outputPath + ".meta";
		
		// Extract GUID from ResourceName if available
		string resStr = resName.GetPath();
		string guid = "";
		
		if (resStr != string.Empty)
		{
			int braceOpen = resStr.IndexOf("{");
			int braceClose = resStr.IndexOf("}");
			if (braceOpen != -1 && braceClose > braceOpen)
			{
				guid = resStr.Substring(braceOpen, braceClose - braceOpen + 1);
			}
		}
		
		// If no GUID found from ResourceName, try to extract from the string representation
		if (guid == "")
		{
			string resNameFull = resName;
			int braceOpen = resNameFull.IndexOf("{");
			int braceClose = resNameFull.IndexOf("}");
			if (braceOpen != -1 && braceClose > braceOpen)
			{
				guid = resNameFull.Substring(braceOpen, braceClose - braceOpen + 1);
			}
		}
		
		if (guid == "")
			return;
		
		FileHandle metaFile = FileIO.OpenFile(metaPath, FileMode.WRITE);
		if (!metaFile)
			return;
		
		// Write a minimal .meta file that Workbench can recognize
		metaFile.WriteLine("MetaFileClass {");
		metaFile.WriteLine(" GUID \"" + guid + "\"");
		metaFile.WriteLine("}");
		metaFile.Close();
	}
	
	// Export the GUID mapping to a CSV file for debugging
	protected void ExportGuidMap()
	{
		string mapPath = "$profile:Dump/guid_map.csv";
		FileHandle mapFile = FileIO.OpenFile(mapPath, FileMode.WRITE);
		if (!mapFile)
		{
			Print("[ScriptDumper] Cannot create guid_map.csv");
			return;
		}
		
		mapFile.WriteLine("OriginalReference,NewPath");
		
		for (int i = 0; i < m_GuidMap.Count(); i++)
		{
			string key = m_GuidMap.GetKey(i);
			string value = m_GuidMap.GetElement(i);
			mapFile.WriteLine(key + "," + value);
		}
		
		mapFile.Close();
		Print("[ScriptDumper] GUID map exported to: " + mapPath + " (" + m_GuidMap.Count() + " entries)");
	}
	
	// Rewrite ResourceName/GUID references in all dumped text files
	protected void RewriteGuidReferences()
	{
		for (int f = 0; f < m_TextFilesToRewrite.Count(); f++)
		{
			string outputPath = m_TextFilesToRewrite[f];
			
			// Read all lines from the file
			FileHandle inputFile = FileIO.OpenFile(outputPath, FileMode.READ);
			if (!inputFile)
				continue;
			
			array<string> lines = {};
			string line;
			bool hasChanges = false;
			
			while (inputFile.ReadLine(line) >= 0)
			{
				string originalLine = line;
				
				// Replace all ResourceName references found in the line
				// Enfusion ResourceName patterns:
				// 1. "{GUID}addon:path/to/file.ext" (full ResourceName with GUID)
				// 2. "$addon:path/to/file.ext" (path reference)
				// We need to replace these with the new path
				
				line = RewriteLineReferences(line);
				
				if (line != originalLine)
					hasChanges = true;
				
				lines.Insert(line);
			}
			inputFile.Close();
			
			// Only rewrite the file if there were changes
			if (hasChanges)
			{
				FileHandle outFile = FileIO.OpenFile(outputPath, FileMode.WRITE);
				if (outFile)
				{
					for (int i = 0; i < lines.Count(); i++)
					{
						outFile.WriteLine(lines[i]);
					}
					outFile.Close();
					m_iRewrittenCount++;
				}
			}
		}
	}
	
	// Rewrite ResourceName references in a single line
	protected string RewriteLineReferences(string line)
	{
		// Strategy: Look for patterns like {GUID}$addon:path or $addon:path
		// and replace the addon:path part with the new relative path
		
		string result = line;
		
		// Pattern 1: Handle "{GUID}$addon:path/file.ext" or "{GUID}addon:path/file.ext"
		// These appear in .et, .emat, .layout files as resource references
		// We want to replace "$oldaddon:old/path" with the target addon path
		
		// Iterate through all known original paths and replace them
		for (int i = 0; i < m_GuidMap.Count(); i++)
		{
			string originalRef = m_GuidMap.GetKey(i);
			string newPath = m_GuidMap.GetElement(i);
			
			if (originalRef == string.Empty || newPath == string.Empty)
				continue;
			
			// Skip if this key is not present in the line (quick check)
			if (!result.Contains(originalRef))
				continue;
			
			// Build the new reference
			string newRef;
			if (m_TargetAddonName != string.Empty)
				newRef = "$" + m_TargetAddonName + ":" + newPath;
			else
				newRef = newPath;
			
			result = ReplaceAll(result, originalRef, newRef);
		}
		
		return result;
	}
	
	// Replace all occurrences of 'search' with 'replace' in 'source'
	protected string ReplaceAll(string source, string search, string replace)
	{
		if (search == string.Empty)
			return source;
		
		string result = source;
		int searchLen = search.Length();
		int replaceLen = replace.Length();
		int startIdx = 0;
		
		while (true)
		{
			int idx = result.IndexOfFrom(startIdx, search);
			if (idx == -1)
				break;
			
			string before = "";
			if (idx > 0)
				before = result.Substring(0, idx);
			
			string after = "";
			int afterStart = idx + searchLen;
			if (afterStart < result.Length())
				after = result.Substring(afterStart, result.Length() - afterStart);
			
			result = before + replace + after;
			startIdx = idx + replaceLen;
		}
		
		return result;
	}
	
	// Creates all parent directories for the given relative path
	protected void EnsureDirectoryExists(string localPath)
	{
		array<string> folders = {};
		localPath.Split("/", folders, true);
		
		// Last element is the filename, not a folder
		int foldersCount = folders.Count() - 1;
		string currentPath = "$profile:";

		for (int i = 0; i < foldersCount; i++)
		{
			currentPath += folders[i];
			if (!FileIO.FileExist(currentPath))
				FileIO.MakeDirectory(currentPath);

			currentPath += "/";
		}
	}
	
	// Copies a resource file to the output path (binary-safe)
	protected void WriteFile(string filePath, ResourceName resName, string outputPath)
	{
		// Determine the best input path to use
		// filePath from callback is always valid (e.g. "$core:UI/layouts/Menus/file.layout")
		// resName.GetPath() can be empty for some resource types
		string inputPath = filePath;
		string resPath = resName.GetPath();
		
		// Attempt 1: Direct binary copy with filePath
		if (FileIO.CopyFile(inputPath, outputPath))
		{
			m_iDumpedCount++;
			return;
		}
		
		// Attempt 2: Direct binary copy with resName path (if different and not empty)
		if (resPath != string.Empty && resPath != inputPath)
		{
			if (FileIO.CopyFile(resPath, outputPath))
			{
				m_iDumpedCount++;
				return;
			}
		}
		
		// Attempt 3: Manual file copy with filePath
		FileHandle inputFile = FileIO.OpenFile(inputPath, FileMode.READ);
		
		// If filePath didn't work, try resName path
		if (!inputFile && resPath != string.Empty && resPath != inputPath)
		{
			inputFile = FileIO.OpenFile(resPath, FileMode.READ);
			inputPath = resPath;
		}
		
		if (!inputFile)
		{
			PrintFormat("[ScriptDumper] Cannot read (tried all paths): %1", filePath);
			m_iErrorCount++;
			return;
		}
		
		FileHandle outputFile = FileIO.OpenFile(outputPath, FileMode.WRITE);
		if (!outputFile)
		{
			PrintFormat("[ScriptDumper] Cannot create: %1", outputPath);
			inputFile.Close();
			m_iErrorCount++;
			return;
		}
		
		// Read the entire file size and copy in binary chunks
		int fileSize = inputFile.GetLength();
		if (fileSize > 0)
		{
			int bufferSize = 4096;
			int bytesRemaining = fileSize;
			
			while (bytesRemaining > 0)
			{
				int toRead = bytesRemaining;
				if (toRead > bufferSize)
					toRead = bufferSize;
				
				string data;
				int bytesRead = inputFile.Read(data, toRead);
				if (bytesRead <= 0)
					break;
				
				outputFile.Write(data, bytesRead);
				bytesRemaining -= bytesRead;
			}
		}
		else
		{
			// fileSize unknown or 0: fallback to line-by-line (text files only)
			string line;
			while (inputFile.ReadLine(line) >= 0)
			{
				outputFile.WriteLine(line);
			}
		}

		inputFile.Close();
		outputFile.Close();
		m_iDumpedCount++;
	}
	
	// Builds the list of enabled file extensions
	protected void GetEnabledFileExtensions(out notnull array<string> extensions)
	{
		// Map each toggle to its extension for cleaner iteration
		array<bool> toggles = {
			m_acp, m_ae, m_agf, m_agr, m_anm, m_asi, m_ast, m_asy, m_aw,
			m_bt, m_c, m_conf, m_ct,
			m_emat, m_et, m_fnt, m_gamemat, m_json,
			m_layer, m_layout,
			m_pap, m_physmat, m_ptc,
			m_sig, m_siga, m_styles,
			m_terr, m_vhcsurf, m_ent, m_edds,
			m_xob, m_imageset, m_wav
		};
		
		array<string> extNames = {
			"acp", "ae", "agf", "agr", "anm", "asi", "ast", "asy", "aw",
			"bt", "c", "conf", "ct",
			"emat", "et", "fnt", "gamemat", "json",
			"layer", "layout",
			"pap", "physmat", "ptc",
			"sig", "siga", "styles",
			"terr", "vhcsurf", "ent", "edds",
			"xob", "imageset", "wav"
		};
		
		for (int i = 0; i < toggles.Count(); i++)
		{
			if (toggles[i])
				extensions.Insert(extNames[i]);
		}
	}
	
	override void Configure()
	{
		Workbench.ScriptDialog("Script Dump Manager - Settings", "Select file types and options, then click Dump.", this);
	}
	
	override void Run()
	{
		Workbench.ScriptDialog("Script Dump Manager - Settings", "Select file types and options, then click Dump.", this);
		super.Run();
	}
}
