#ifndef __ProgramInterface_h__
#define __ProgramInterface_h__

void Run(const char * const historicFilename,
         const char * const detailFilename,
         const bool verboseOn,
         const bool outputToFile,
         const bool generateReport,
         unsigned int samples,
         unsigned int leavesToSample);

#endif // __ProgramInterface_h__
