package org.gnu.pilotlink;
import java.util.*;

public class RawRecord extends Record {
	private byte[] buffer;
	
	public RawRecord(byte[] b,long i, int sz, int attr, int cat) {
		buffer=b;
		setCategory(cat);
		setAttribs(attr);
		setSize(sz);
		setId(i);		
	}
	public RawRecord(byte[] b, int sz, int attr, int cat) {
		this(b,0,sz,attr,cat);
	}
    public byte[] getBuffer() {
        return buffer;
    }
    public void setBuffer(byte buf[]) {
        buffer=buf;
    }
}
