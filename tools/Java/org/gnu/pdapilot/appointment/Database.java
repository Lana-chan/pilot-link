/* 
 * appointment/Database.java:
 *
 * Copyright (C) 1997, 1998, Kenneth Albanowski
 * Copyright (C) 2001 David.Goodenough@DGA.co.uk
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

package org.gnu.pdapilot.appointment;

public class Database extends org.gnu.pdapilot.Database {
   public org.gnu.pdapilot.AppBlock newAppBlock( ) { 
      return new org.gnu.pdapilot.appointment.AppBlock( ); 
      }
   public org.gnu.pdapilot.AppBlock newAppBlock( byte[ ] contents) { 
      return new org.gnu.pdapilot.appointment.AppBlock( contents); 
      }
   public org.gnu.pdapilot.Record newRecord( ) { 
      return new org.gnu.pdapilot.appointment.Record( ); 
      }
   public org.gnu.pdapilot.Record newRecord( byte[ ]contents, org.gnu.pdapilot.RecordID id, int index, int attr, int cat) { 
      return new org.gnu.pdapilot.appointment.Record( contents, id, index, attr, cat); 
      }
   public org.gnu.pdapilot.Char4 creator( ) { return new org.gnu.pdapilot.Char4( "date"); }
   public String dbname( ) { return "DatebookDB"; }
   }
