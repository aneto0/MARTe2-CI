/**
 * @file HttpDirectoryResource.cpp
 * @brief Source file for class HttpDirectoryResource
 * @date 31/10/2018
 * @author Andre Neto
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This source file contains the definition of all the methods for
 * the class HttpDirectoryResource (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "Directory.h"
#include "DirectoryScanner.h"
#include "HttpDirectoryResource.h"
#include "HttpDefinition.h"
#include "JsonPrinter.h"
#include "StreamStructuredData.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

HttpDirectoryResource::HttpDirectoryResource() {

}

HttpDirectoryResource::~HttpDirectoryResource() {

}

bool HttpDirectoryResource::Initialise(StructuredDataI &data) {
    bool ok = Object::Initialise(data);
    if (ok) {
        ok = data.Read("BaseDir", baseDir);
        if (ok) {
            REPORT_ERROR(ErrorManagement::Information, "Serving directory %s", baseDir.Buffer());
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "The BaseDir directory shall be specified");
        }
    }
    return ok;
}

bool HttpDirectoryResource::GetAsStructuredData(StreamStructuredDataI &data, HttpProtocol &protocol) {
    bool ok = HttpDataExportI::GetAsStructuredData(data, protocol);
    if (ok) {
        StreamStructuredData<JsonPrinter> *sdata = dynamic_cast<StreamStructuredData<JsonPrinter> *>(&data);
        ok = (sdata != NULL_PTR(StreamStructuredData<JsonPrinter> *));
        if (ok) {
            sdata->GetPrinter()->PrintBegin();
        }
        if (ok) {
            ok = Object::ExportData(data);
        }
        if (ok) {
            StreamString path;
            (void) protocol.GetInputCommand("path", path);
            StreamString fullPath = baseDir;
            fullPath += DIRECTORY_SEPARATOR;
            fullPath += path.Buffer();
            File f;
            Directory d(fullPath.Buffer());
            ok = data.CreateRelative("Files");
            if (ok) {
                if (d.IsDirectory()) {
                    DirectoryScanner ds;
                    ok = ds.Scan(fullPath.Buffer());
                    uint32 i = 0;
                    while ((ds.ListPeek(i) != NULL) && (ok)) {
                        Directory *dt = static_cast<Directory *>(ds.ListPeek(i));
                        if (dt != NULL_PTR(Directory *)) {
                            StreamString idx;
                            idx.Printf("%d", i);
                            ok = data.CreateRelative(idx.Buffer());
                            if (ok) {
                                ok = data.Write("Name", dt->GetName());
                            }
                            if (ok) {
                                ok = data.Write("IsDirectory", dt->IsDirectory() ? "1" : "0");
                            }
                            if (ok) {
                                StreamString ss;
                                (void) ss.Printf("%d", dt->GetSize());
                                ok = data.Write("Size", ss.Buffer());
                            }
                            if (ok) {
                                ok = data.MoveToAncestor(1u);
                            }
                        }
                        i++;
                    }
                }
                else {
                    ok = data.CreateRelative("0");
                    if (ok) {
                        ok = data.Write("Name", d.GetName());
                    }
                    if (ok) {
                        ok = data.Write("IsDirectory", "0");
                    }
                    if (ok) {
                        StreamString ss;
                        (void) ss.Printf("%d", d.GetSize());
                        ok = data.Write("Size", ss.Buffer());
                    }
                    if (ok) {
                        ok = data.MoveToAncestor(1u);
                    }
                }
            }
            if (ok) {
                ok = data.MoveToAncestor(1u);
            }
        }
        if (ok) {
            ok = sdata->GetPrinter()->PrintEnd();
        }
    }
    return ok;
}

bool HttpDirectoryResource::GetAsText(StreamI &stream, HttpProtocol &protocol) {
    StreamString path;
    if (!protocol.GetInputCommand("path", path)) {
        path = "index.html";
    }
    StreamString fullPath = baseDir;
    fullPath += DIRECTORY_SEPARATOR;
    fullPath += path.Buffer();
    REPORT_ERROR(ErrorManagement::Debug, "Serving %s", fullPath.Buffer());
    bool ok = ServeFile(fullPath, stream, protocol);
    return ok;
}

bool HttpDirectoryResource::CheckExtension(StreamString &fname, const char8 * const ext) {
    uint32 extLen = StringHelper::Length(ext);
    bool ok = (fname.Size() >= extLen);
    if (ok) {
        const char8 * fnameb = fname.Buffer();
        uint32 fnameNoExtIdx = (fname.Size() - extLen);
        ok = (StringHelper::CompareNoCaseSensN(&fnameb[fnameNoExtIdx], ext, extLen) == 0u);
    }
    return ok;
}

bool HttpDirectoryResource::ServeFile(StreamString &fname, StreamI &stream, HttpProtocol &protocol) {
    File f;
    bool ok = f.Open(fname.Buffer(), MARTe::BasicFile::ACCESS_MODE_R);
    StreamString mime = "binary";

    if (ok) {
        if (CheckExtension(fname, ".html") || CheckExtension(fname, ".htm")) {
            mime = "text/html";
        }
        else if (CheckExtension(fname, ".txt")) {
            mime = "text/plain";
        }
        else if (CheckExtension(fname, ".csv")) {
            mime = "text/csv";
        }
        else if (CheckExtension(fname, ".css")) {
            mime = "text/css";
        }
        else if (CheckExtension(fname, ".gif")) {
            mime = "image/gif";
        }
        else if (CheckExtension(fname, ".jpeg") || CheckExtension(fname, ".jpg")) {
            mime = "image/jpg";
        }
        else if (CheckExtension(fname, ".jnlp")) {
            mime = "image/jnlp";
        }
        else if (CheckExtension(fname, ".js")) {
            mime = "application/x-javascript";
        }
        else {
            mime = "binary";
        }
        if (ok) {
            ok = protocol.MoveAbsolute("OutputOptions");
        }
    }
    if (ok) {
        ok = protocol.Write("Content-Type", mime.Buffer());
    }
    if (ok) {
        //With the HEAD just inform that the file exists
        if (protocol.GetHttpCommand() == HttpDefinition::HSHCHead) {
            StreamString s;
            s.SetSize(0LLU);
            ok = protocol.WriteHeader(true, HttpDefinition::HSHCReplyOK, &s, NULL_PTR(const char8*));
        }
        else {
            ok = protocol.WriteHeader(true, HttpDefinition::HSHCReplyOK, &f, NULL_PTR(const char8*));
        }
    }
    /*if (ok) {
     ok = f.Size()Seek(0LLU);
     }
     const uint32 bufReadSize = 128000u;
     char8 *buf = NULL_PTR(char8 *);
     if (ok) {
     buf = reinterpret_cast<char8 *>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(bufReadSize));
     uint32 bufSize = 128000u;
     while ((f.Read(buf, bufSize)) && (ok)) {
     ok = stream.Write(buf, bufSize);
     bufSize = bufReadSize;
     }
     }
     if (ok) {
     if (buf != NULL_PTR(char8 *)) {
     GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(reinterpret_cast<void *&>(buf));
     }
     }
     if (ok) {
     ok = f.Close();
     }*/
    if (ok) {
        ok = f.Close();
    }
    return ok;
}

CLASS_REGISTER(HttpDirectoryResource, "1.0")
}
