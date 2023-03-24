/* Funcheck - A tool for checking functions calls return protections
 * Copyright (C) 2023  Theo Matis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
