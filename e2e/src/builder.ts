import childProcess from 'child_process';
import * as fs from 'fs';

export type BuilderProperty = {
  cc: string;
  flags: string[];
};

export class Builder {
  constructor(private readonly _property: BuilderProperty) {}

  public build(
    sourceFileNames: string[],
    outputFileName: string
  ): string {
    // check if file exists
    for (const sourceFileName of sourceFileNames) {
      if (!fs.existsSync(sourceFileName)) {
        throw new Error(`File ${sourceFileName} does not exist`);
      }
    }

    const args = [
      ...this._property.flags,
      ...sourceFileNames,
      '-o',
      outputFileName,
    ];
    const child = childProcess.spawnSync(this._property.cc, args, {
      cwd: process.cwd(),
    });

    if (child.status !== 0) {
      throw new Error(
        `Failed to build: ${child.stderr?.toString() || ''}`
      );
    }

    return outputFileName;
  }
}
