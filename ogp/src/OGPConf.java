package ogp;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Scanner;
import java.util.Set;

public class OGPConf {

    private final static String MAJOR_VERSION = "0";
    private final static String MINOR_VERSION = "0";
    private final static String VERSION = MAJOR_VERSION + "." + MINOR_VERSION;

    private final static String CONF_FILE = "/usr/local/share/ogp/ogp.conf";

    private int nrProvers = 0;
    private Map<String, OGPProverInfo> proversInfo = new HashMap<String,
	OGPProverInfo>();
    private Set<String> proversExt = new HashSet<String>();

    public OGPConf() {
	readConfFile(CONF_FILE);
	readConfFile(System.getenv("HOME")
		     + System.getProperty("file.separator")
		     + ".ogprc");
	if (proversInfo.size() == 0) {
	    errorMsg(12, "");
	}
    }

    public String getVersion() {
	return this.VERSION;
    }

    public int getNrProvers() {
	return this.nrProvers;
    }

    public Map<String, OGPProverInfo> getProversInfo() {
	return this.proversInfo;
    }

    public Set<String> getProversExt() {
	return this.proversExt;
    }

    public Set<String> getProversSet() {
	return this.proversInfo.keySet();
    }

    private void readConfFile(String file) {
	try {
	    File confFile = new File(file);
	    if (confFile.canRead() && confFile.isFile()) {
		Scanner confFileScanner = new Scanner(confFile);
		while (confFileScanner.hasNextLine()) {
		    String line = confFileScanner.nextLine();
		    if (!line.startsWith("#") && !line.matches("\\s*")) {
			// Determine OGP prover's identification
			int begIndex = 0;
			int endIndex = line.indexOf(":", begIndex);
			String ogpId = line.substring(begIndex, endIndex);
			if (ogpId.isEmpty()) {
			    errorMsg(11, file + ": no prover identification");
			}
			// Determine prover's command line command
			begIndex = ++endIndex;
			endIndex = line.indexOf(":", begIndex);
			String cmd = line.substring(begIndex, endIndex);
			if (cmd.isEmpty()) {
			    errorMsg(11, file + ": no prover command");
			}
			// Determine prover's conjectuture file extension
			begIndex = ++endIndex;
			endIndex = line.indexOf(":", begIndex);
			String ext = line.substring(begIndex, endIndex);
			if (ext.isEmpty()) {
			    errorMsg(11, file + ": no prover extension");
			}
			// Determine prover's ext2FOF command, if any
			begIndex = ++endIndex;
			endIndex = line.indexOf(":", begIndex);
			String toFOFCmd = line.substring(begIndex, endIndex);
			// Determine prover's FOF2ext command, if any
			begIndex = ++endIndex;
			endIndex = line.indexOf(":", begIndex);
			String toExtCmd = line.substring(begIndex, endIndex);
			// Determine prover's post-processing command, if any
			begIndex = ++endIndex;
			endIndex = line.indexOf(":", begIndex);
			String postProcCmd = line.substring(begIndex, endIndex);
			// Determine prover's name, if any
			begIndex = ++endIndex;
			endIndex = line.indexOf(":", begIndex);
			String name = line.substring(begIndex, endIndex);
			// Determine prover's description, if any
			String desc = line.substring(++endIndex);

			proversInfo.put(ogpId, new OGPProverInfo(cmd,
								 ext,
								 toFOFCmd,
								 toExtCmd,
								 postProcCmd,
								 name,
								 desc));
			proversExt.add(ext);
			nrProvers++;
		    }
		}
		confFileScanner.close();
	    }
	} catch (FileNotFoundException e) {
	    errorMsg(99, e.toString());
	} catch (StringIndexOutOfBoundsException e) {
	    errorMsg(11, file + ": wrong sintax");
	}
    }

    private static void errorMsg(int error, String str) {
	System.err.print("[ERROR " + error + "] (OGPConf) ");
	switch (error) {
	case 11:
	    System.err.println("Problem with configuration file " + str);
	    break;
	case 12:
	    System.err.println("OGP has no information about provers");
	    break;
	case 99:
	    System.err.println("Something is really wrong :-|");
	    System.err.println(str);
	    break;
	}
	System.exit(error);
    }

}
