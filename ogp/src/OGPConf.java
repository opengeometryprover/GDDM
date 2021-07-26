package ogp;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.HashMap;
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

    public Set<String> getProversSet() {
	return this.proversInfo.keySet();
    }

    public Map<String, OGPProverInfo> getProversInfo() {
	return this.proversInfo;
    }

    private void readConfFile(String file) {
	try {
	    File confFile = new File(file);
	    if (confFile.canRead() && confFile.isFile()) {
		Scanner confFileScanner = new Scanner(confFile);
		while (confFileScanner.hasNextLine()) {
		    String line = confFileScanner.nextLine();

		    int beginIndex = 0;
		    int endIndex = line.indexOf(":", beginIndex);
		    String ogpId = line.substring(beginIndex, endIndex);
		    if (ogpId.isEmpty()) {
			errorMsg(11, file + ": missing prover identification");
		    }

		    beginIndex = ++endIndex;
		    endIndex = line.indexOf(":", beginIndex);
		    String cmd = line.substring(beginIndex, endIndex);
		    if (cmd.isEmpty()) {
			errorMsg(11, file + ": missing prover command");
		    }

		    beginIndex = ++endIndex;
		    endIndex = line.indexOf(":", beginIndex);
		    String ext = line.substring(beginIndex, endIndex);
		    if (ext.isEmpty()) {
			errorMsg(11, file + ": missing prover extension");
		    }

		    beginIndex = ++endIndex;
		    endIndex = line.indexOf(":", beginIndex);
		    String toFOFCmd = line.substring(beginIndex, endIndex);

		    beginIndex = ++endIndex;
		    endIndex = line.indexOf(":", beginIndex);
		    String toExtCmd = line.substring(beginIndex, endIndex);

		    beginIndex = ++endIndex;
		    endIndex = line.indexOf(":", beginIndex);
		    String postProcCmd = line.substring(beginIndex, endIndex);

		    beginIndex = ++endIndex;
		    endIndex = line.indexOf(":", beginIndex);
		    String name = line.substring(beginIndex, endIndex);

		    String desc = line.substring(++endIndex);

		    proversInfo.put(ogpId, new OGPProverInfo(cmd,
							     ext,
							     toFOFCmd,
							     toExtCmd,
							     postProcCmd,
							     name,
							     desc));
		    nrProvers++;
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
