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

import { Builder } from './builder';
import childProcess from 'child_process';

export class Runner {
  /**
   * @param args - Arguments to pass to the program
   *
   * @returns The output of the program
   */
  public run(program: string, args: string[]): string {
    const child = childProcess.spawnSync(program, args, {
      cwd: process.cwd(),
    });

    return child.stdout?.toString() || '';
  }
}
